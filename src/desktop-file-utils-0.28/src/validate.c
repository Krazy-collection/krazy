/* validate.c: validate a desktop entry file
 * vim: set ts=2 sw=2 et: */

/*
 * Copyright (C) 2007-2009 Vincent Untz <vuntz@gnome.org>
 *
 * A really small portion of this code comes from the old validate.c.
 * The old validate.c was Copyright (C) 2002, 2004  Red Hat, Inc.
 * It was written by:
 *  Mark McLoughlin <mark@skynet.ie>
 *  Havoc Pennington <hp@pobox.com>
 *  Ray Strode <rstrode@redhat.com>
 *
 * A portion of this code comes from glib (gkeyfile.c)
 * Authors of gkeyfile.c are:
 *  Ray Strode
 *  Matthias Clasen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>

#include "keyfileutils.h"
#include "mimeutils.h"
#include "validate.h"

/*FIXME: document where we are stricter than the spec
 * + only UTF-8 (so no Legacy-Mixed encoding)
 */

/*TODO:
 * + Lecagy-Mixed Encoding (annexe D)
 * + The escape sequences \s, \n, \t, \r, and \\ are supported for values of
 *   type string and localestring, meaning ASCII space, newline, tab, carriage
 *   return, and backslash, respectively.
 */

typedef enum {
  INVALID_TYPE = 0,

  APPLICATION_TYPE,
  LINK_TYPE,
  DIRECTORY_TYPE,

  /* Types reserved for KDE */
  /* since 0.9.4 */
  SERVICE_TYPE,
  SERVICE_TYPE_TYPE,
  /* since 0.9.6 */
  FSDEVICE_TYPE,

  /* Deprecated types */
  /* since 0.9.4 */
  MIMETYPE_TYPE,

  LAST_TYPE
} DesktopType;

typedef enum {
  DESKTOP_STRING_TYPE,
  DESKTOP_LOCALESTRING_TYPE,
  DESKTOP_BOOLEAN_TYPE,
  DESKTOP_NUMERIC_TYPE,
  DESKTOP_STRING_LIST_TYPE,
  DESKTOP_LOCALESTRING_LIST_TYPE,
  /* Deprecated types */
  /* since 0.9.6 */
  DESKTOP_REGEXP_LIST_TYPE
} DesktopKeyType;

typedef struct _kf_keyvalue kf_keyvalue;

struct _kf_keyvalue {
  char *key;
  char *value;
};

typedef struct _kf_validator kf_validator;

struct _kf_validator {
  const char  *filename;

  GString     *parse_buffer;
  gboolean     utf8_warning;
  gboolean     cr_error;

  char        *current_group;
  GHashTable  *groups;
  GHashTable  *current_keys;

  gboolean     kde_reserved_warnings;
  gboolean     no_deprecated_warnings;
  gboolean     no_hints;

  char        *main_group;
  DesktopType  type;
  char        *type_string;

  gboolean     show_in;
  GList       *application_keys;
  GList       *link_keys;
  GList       *fsdevice_keys;
  GList       *mimetype_keys;

  GHashTable  *interfaces;
  GHashTable  *action_values;
  GHashTable  *action_groups;

  gboolean     fatal_error;

  gboolean     use_colors;
};

static gboolean
validate_string_key (kf_validator *kf,
                     const char   *key,
                     const char   *locale,
                     const char   *value);
static gboolean
validate_localestring_key (kf_validator *kf,
                           const char   *key,
                           const char   *locale,
                           const char   *value);
static gboolean
validate_boolean_key (kf_validator *kf,
                      const char   *key,
                      const char   *locale,
                      const char   *value);
static gboolean
validate_numeric_key (kf_validator *kf,
                      const char   *key,
                      const char   *locale,
                      const char   *value);
static gboolean
validate_string_list_key (kf_validator *kf,
                          const char   *key,
                          const char   *locale,
                          const char   *value);
static gboolean
validate_regexp_list_key (kf_validator *kf,
                          const char   *key,
                          const char   *locale,
                          const char   *value);
static gboolean
validate_localestring_list_key (kf_validator *kf,
                                const char   *key,
                                const char   *locale,
                                const char   *value);

static gboolean
handle_type_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value);
static gboolean
handle_version_key (kf_validator *kf,
                    const char   *locale_key,
                    const char   *value);
static gboolean
handle_comment_key (kf_validator *kf,
                    const char   *locale_key,
                    const char   *value);
static gboolean
handle_icon_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value);
static gboolean
handle_show_in_key (kf_validator *kf,
                    const char   *locale_key,
                    const char   *value);
static gboolean
handle_desktop_exec_key (kf_validator *kf,
                         const char   *locale_key,
                         const char   *value);
static gboolean
handle_exec_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value);
static gboolean
handle_path_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value);
static gboolean
handle_mime_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value);
static gboolean
handle_categories_key (kf_validator *kf,
                       const char   *locale_key,
                       const char   *value);
static gboolean
handle_actions_key (kf_validator *kf,
                    const char   *locale_key,
                    const char   *value);
static gboolean
handle_implements_key (kf_validator *kf,
                       const char   *locale_key,
                       const char   *value);
static gboolean
handle_dbus_activatable_key (kf_validator *kf,
                             const char   *locale_key,
                             const char   *value);
static gboolean
handle_dev_key (kf_validator *kf,
                const char   *locale_key,
                const char   *value);
static gboolean
handle_mountpoint_key (kf_validator *kf,
                       const char   *locale_key,
                       const char   *value);
static gboolean
handle_encoding_key (kf_validator *kf,
                     const char   *locale_key,
                     const char   *value);
static gboolean
handle_autostart_condition_key (kf_validator *kf,
                                const char   *locale_key,
                                const char   *value);
static gboolean
handle_key_for_application (kf_validator *kf,
                            const char   *locale_key,
                            const char   *value);
static gboolean
handle_key_for_link (kf_validator *kf,
                     const char   *locale_key,
                     const char   *value);
static gboolean
handle_key_for_fsdevice (kf_validator *kf,
                         const char   *locale_key,
                         const char   *value);
static gboolean
handle_key_for_mimetype (kf_validator *kf,
                         const char   *locale_key,
                         const char   *value);

static struct {
  DesktopType  type;
  char        *name;
  gboolean     kde_reserved;
  gboolean     deprecated;
} registered_types[] = {
  { APPLICATION_TYPE,  "Application", FALSE, FALSE },
  { LINK_TYPE,         "Link",        FALSE, FALSE },
  { DIRECTORY_TYPE,    "Directory",   FALSE, FALSE },
  { SERVICE_TYPE,      "Service",     TRUE,  FALSE },
  { SERVICE_TYPE_TYPE, "ServiceType", TRUE,  FALSE },
  { FSDEVICE_TYPE,     "FSDevice",    TRUE,  FALSE },
  { MIMETYPE_TYPE,     "MimeType",    FALSE, TRUE  }
};

static struct {
  DesktopKeyType type;
  gboolean       (* validate) (kf_validator *kf,
                               const char   *key,
                               const char   *locale,
                               const char   *value);
} validate_for_type[] = {
  { DESKTOP_STRING_TYPE,            validate_string_key            },
  { DESKTOP_LOCALESTRING_TYPE,      validate_localestring_key      },
  { DESKTOP_BOOLEAN_TYPE,           validate_boolean_key           },
  { DESKTOP_NUMERIC_TYPE,           validate_numeric_key           },
  { DESKTOP_STRING_LIST_TYPE,       validate_string_list_key       },
  { DESKTOP_REGEXP_LIST_TYPE,       validate_regexp_list_key       },
  { DESKTOP_LOCALESTRING_LIST_TYPE, validate_localestring_list_key }
};

typedef struct {
  DesktopKeyType  type;
  char           *name;
  gboolean        required;
  gboolean        deprecated;
  gboolean        kde_reserved;
  gboolean        (* handle_and_validate) (kf_validator *kf,
                                           const char   *locale_key,
                                           const char   *value);
} DesktopKeyDefinition;

static DesktopKeyDefinition registered_desktop_keys[] = {
  { DESKTOP_STRING_TYPE,            "Type",              TRUE,  FALSE, FALSE, handle_type_key },
  /* it is numeric according to the spec, but it's not true in previous
   * versions of the spec. handle_version_key() will manage this */
  { DESKTOP_STRING_TYPE,            "Version",           FALSE, FALSE, FALSE, handle_version_key },
  { DESKTOP_LOCALESTRING_TYPE,      "Name",              TRUE,  FALSE, FALSE, NULL },
  { DESKTOP_LOCALESTRING_TYPE,      "GenericName",       FALSE, FALSE, FALSE, NULL },
  { DESKTOP_BOOLEAN_TYPE,           "NoDisplay",         FALSE, FALSE, FALSE, NULL },
  { DESKTOP_LOCALESTRING_TYPE,      "Comment",           FALSE, FALSE, FALSE, handle_comment_key },
  { DESKTOP_LOCALESTRING_TYPE,      "Icon",              FALSE, FALSE, FALSE, handle_icon_key },
  { DESKTOP_BOOLEAN_TYPE,           "Hidden",            FALSE, FALSE, FALSE, NULL },
  { DESKTOP_STRING_LIST_TYPE,       "OnlyShowIn",        FALSE, FALSE, FALSE, handle_show_in_key },
  { DESKTOP_STRING_LIST_TYPE,       "NotShowIn",         FALSE, FALSE, FALSE, handle_show_in_key },
  { DESKTOP_STRING_TYPE,            "TryExec",           FALSE, FALSE, FALSE, handle_key_for_application },
  { DESKTOP_STRING_TYPE,            "Exec",              FALSE, FALSE, FALSE, handle_desktop_exec_key },
  { DESKTOP_STRING_TYPE,            "Path",              FALSE, FALSE, FALSE, handle_path_key },
  { DESKTOP_BOOLEAN_TYPE,           "Terminal",          FALSE, FALSE, FALSE, handle_key_for_application },
  { DESKTOP_STRING_LIST_TYPE,       "MimeType",          FALSE, FALSE, FALSE, handle_mime_key },
  { DESKTOP_STRING_LIST_TYPE,       "Categories",        FALSE, FALSE, FALSE, handle_categories_key },
  { DESKTOP_BOOLEAN_TYPE,           "StartupNotify",     FALSE, FALSE, FALSE, handle_key_for_application },
  { DESKTOP_STRING_TYPE,            "StartupWMClass",    FALSE, FALSE, FALSE, handle_key_for_application },
  { DESKTOP_STRING_TYPE,            "URL",               FALSE, FALSE, FALSE, handle_key_for_link },
  /* since 1.1 (used to be a key reserved for KDE since 0.9.4) */
  { DESKTOP_LOCALESTRING_LIST_TYPE, "Keywords",          FALSE, FALSE, FALSE, NULL },
  /* since 1.1 (used to be in the spec before 1.0, but was not really
   * specified) */
  { DESKTOP_STRING_LIST_TYPE,       "Actions",           FALSE, FALSE, FALSE, handle_actions_key },
  /* Since 1.2 */
  { DESKTOP_STRING_LIST_TYPE,       "Implements",        FALSE, FALSE, FALSE, handle_implements_key },

  { DESKTOP_BOOLEAN_TYPE,           "DBusActivatable",   FALSE, FALSE, FALSE, handle_dbus_activatable_key },

  /* Since 1.4 */
  { DESKTOP_BOOLEAN_TYPE,           "PrefersNonDefaultGPU", FALSE, FALSE, FALSE, NULL },

  /* Since 1.5 */
  { DESKTOP_BOOLEAN_TYPE,           "SingleMainWindow", FALSE, FALSE, FALSE, NULL },

  /* Keys reserved for KDE */

  /* since 0.9.4 */
  { DESKTOP_STRING_TYPE,            "ServiceTypes",      FALSE, FALSE, TRUE,  NULL },
  { DESKTOP_STRING_TYPE,            "DocPath",           FALSE, FALSE, TRUE,  NULL },
  { DESKTOP_STRING_TYPE,            "InitialPreference", FALSE, FALSE, TRUE,  NULL },
  /* since 0.9.6 */
  { DESKTOP_STRING_TYPE,            "Dev",               FALSE, FALSE, TRUE,  handle_dev_key },
  { DESKTOP_STRING_TYPE,            "FSType",            FALSE, FALSE, TRUE,  handle_key_for_fsdevice },
  { DESKTOP_STRING_TYPE,            "MountPoint",        FALSE, FALSE, TRUE,  handle_mountpoint_key },
  { DESKTOP_BOOLEAN_TYPE,           "ReadOnly",          FALSE, FALSE, TRUE,  handle_key_for_fsdevice },
  { DESKTOP_STRING_TYPE,            "UnmountIcon",       FALSE, FALSE, TRUE,  handle_key_for_fsdevice },

  /* Deprecated keys */

  /* since 0.9.3 */
  { DESKTOP_STRING_TYPE,            "Protocols",         FALSE, TRUE,  FALSE, NULL },
  { DESKTOP_STRING_TYPE,            "Extensions",        FALSE, TRUE,  FALSE, NULL },
  { DESKTOP_STRING_TYPE,            "BinaryPattern",     FALSE, TRUE,  FALSE, NULL },
  { DESKTOP_STRING_TYPE,            "MapNotify",         FALSE, TRUE,  FALSE, NULL },
  /* since 0.9.4 */
  { DESKTOP_REGEXP_LIST_TYPE,       "Patterns",          FALSE, TRUE,  FALSE, handle_key_for_mimetype },
  { DESKTOP_STRING_TYPE,            "DefaultApp",        FALSE, TRUE,  FALSE, handle_key_for_mimetype },
  { DESKTOP_STRING_TYPE,            "MiniIcon",          FALSE, TRUE,  FALSE, NULL },
  { DESKTOP_STRING_TYPE,            "TerminalOptions",   FALSE, TRUE,  FALSE, NULL },
  /* since 0.9.5 */
  { DESKTOP_STRING_TYPE,            "Encoding",          FALSE, TRUE,  FALSE, handle_encoding_key },
  { DESKTOP_LOCALESTRING_TYPE,      "SwallowTitle",      FALSE, TRUE,  FALSE, NULL },
  { DESKTOP_STRING_TYPE,            "SwallowExec",       FALSE, TRUE,  FALSE, NULL },
  /* since 0.9.6 */
  { DESKTOP_STRING_LIST_TYPE,       "SortOrder",         FALSE, TRUE,  FALSE, NULL },
  { DESKTOP_REGEXP_LIST_TYPE,       "FilePattern",       FALSE, TRUE,  FALSE, NULL },
  /* since 1.4 */
  { DESKTOP_BOOLEAN_TYPE,           "X-KDE-RunOnDiscreteGpu", FALSE, TRUE, FALSE, NULL },

  /* Keys from other specifications */

  /* Autostart spec, currently proposed; adopted by GNOME */
  { DESKTOP_STRING_TYPE,            "AutostartCondition", FALSE, FALSE, FALSE, handle_autostart_condition_key }
};

static DesktopKeyDefinition registered_action_keys[] = {
  { DESKTOP_LOCALESTRING_TYPE,      "Name",               TRUE,  FALSE, FALSE, NULL },
  { DESKTOP_LOCALESTRING_TYPE,      "Icon",               FALSE, FALSE, FALSE, handle_icon_key },
  { DESKTOP_STRING_LIST_TYPE,       "OnlyShowIn",         FALSE, TRUE, FALSE, handle_show_in_key },
  { DESKTOP_STRING_LIST_TYPE,       "NotShowIn",          FALSE, TRUE, FALSE, handle_show_in_key },
  { DESKTOP_STRING_TYPE,            "Exec",               TRUE,  FALSE, FALSE, handle_exec_key }
};

/* This should be the same list as in xdg-specs/menu/menu-spec.xml */
static const char *show_in_registered[] = {
    "COSMIC", "GNOME", "GNOME-Classic", "GNOME-Flashback", "KDE", "LXDE", "LXQt", "MATE", "Razor", "ROX", "TDE", "Unity", "XFCE", "EDE", "Cinnamon", "Pantheon", "Budgie", "Enlightenment", "DDE", "Endless", "Old"
};

static struct {
  const char   *name;
  const char   *first_arg[3];
  unsigned int  additional_args;
} registered_autostart_condition[] = {
  { "GNOME",     { NULL }, 1 },
  { "GNOME3",    { "if-session", "unless-session", NULL }, 1},
  { "GSettings", { NULL }, 2 }
};

static struct {
  const char *name;
  gboolean    main;
  gboolean    require_only_show_in;
  gboolean    deprecated;
  const char *requires[2];
  const char *suggests[4];
} registered_categories[] = {
  { "AudioVideo",             TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Audio",                  TRUE,  FALSE, FALSE, { "AudioVideo", NULL }, { NULL } },
  { "Video",                  TRUE,  FALSE, FALSE, { "AudioVideo", NULL }, { NULL } },
  { "Development",            TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Education",              TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Game",                   TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Graphics",               TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Network",                TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Office",                 TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Science",                TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Settings",               TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "System",                 TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Utility",                TRUE,  FALSE, FALSE, { NULL }, { NULL } },
  { "Audio",                  FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "Video",                  FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "Building",               FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "Debugger",               FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "IDE",                    FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "GUIDesigner",            FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "Profiling",              FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "RevisionControl",        FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "Translation",            FALSE, FALSE, FALSE, { NULL }, { "Development", NULL } },
  { "Calendar",               FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "ContactManagement",      FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "Database",               FALSE, FALSE, FALSE, { NULL }, { "Office", "Development", "AudioVideo", NULL } },
  { "Dictionary",             FALSE, FALSE, FALSE, { NULL }, { "Office", "TextTools", NULL } },
  { "Chart",                  FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "Email",                  FALSE, FALSE, FALSE, { NULL }, { "Office", "Network", NULL } },
  { "Finance",                FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "FlowChart",              FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "PDA",                    FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "ProjectManagement",      FALSE, FALSE, FALSE, { NULL }, { "Office", "Development", NULL } },
  { "Presentation",           FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "Spreadsheet",            FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "WordProcessor",          FALSE, FALSE, FALSE, { NULL }, { "Office", NULL } },
  { "2DGraphics",             FALSE, FALSE, FALSE, { NULL }, { "Graphics", NULL } },
  { "VectorGraphics",         FALSE, FALSE, FALSE, { NULL }, { "Graphics;2DGraphics", NULL } },
  { "RasterGraphics",         FALSE, FALSE, FALSE, { NULL }, { "Graphics;2DGraphics", NULL } },
  { "3DGraphics",             FALSE, FALSE, FALSE, { NULL }, { "Graphics", NULL } },
  { "Scanning",               FALSE, FALSE, FALSE, { NULL }, { "Graphics", NULL } },
  { "OCR",                    FALSE, FALSE, FALSE, { NULL }, { "Graphics;Scanning", NULL } },
  { "Photography",            FALSE, FALSE, FALSE, { NULL }, { "Graphics", "Office", NULL } },
  { "Publishing",             FALSE, FALSE, FALSE, { NULL }, { "Graphics", "Office", NULL } },
  { "Viewer",                 FALSE, FALSE, FALSE, { NULL }, { "Graphics", "Office", NULL } },
  { "TextTools",              FALSE, FALSE, FALSE, { NULL }, { "Utility", NULL } },
  { "DesktopSettings",        FALSE, FALSE, FALSE, { NULL }, { "Settings", NULL } },
  { "HardwareSettings",       FALSE, FALSE, FALSE, { NULL }, { "Settings", NULL } },
  { "Printing",               FALSE, FALSE, FALSE, { NULL }, { "HardwareSettings;Settings", NULL } },
  { "PackageManager",         FALSE, FALSE, FALSE, { NULL }, { "Settings", NULL } },
  { "Dialup",                 FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "InstantMessaging",       FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "Chat",                   FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "IRCClient",              FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "Feed",                   FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "FileTransfer",           FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "HamRadio",               FALSE, FALSE, FALSE, { NULL }, { "Network", "Audio", NULL } },
  { "News",                   FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "P2P",                    FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "RemoteAccess",           FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "Telephony",              FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "TelephonyTools",         FALSE, FALSE, FALSE, { NULL }, { "Utility", NULL } },
  { "VideoConference",        FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "WebBrowser",             FALSE, FALSE, FALSE, { NULL }, { "Network", NULL } },
  { "WebDevelopment",         FALSE, FALSE, FALSE, { NULL }, { "Network", "Development", NULL } },
  { "Midi",                   FALSE, FALSE, FALSE, { NULL }, { "AudioVideo;Audio", NULL } },
  { "Mixer",                  FALSE, FALSE, FALSE, { NULL }, { "AudioVideo;Audio", NULL } },
  { "Sequencer",              FALSE, FALSE, FALSE, { NULL }, { "AudioVideo;Audio", NULL } },
  { "Tuner",                  FALSE, FALSE, FALSE, { NULL }, { "AudioVideo;Audio", NULL } },
  { "TV",                     FALSE, FALSE, FALSE, { NULL }, { "AudioVideo;Video", NULL } },
  { "AudioVideoEditing",      FALSE, FALSE, FALSE, { NULL }, { "Audio", "Video", "AudioVideo", NULL } },
  { "Player",                 FALSE, FALSE, FALSE, { NULL }, { "Audio", "Video", "AudioVideo", NULL } },
  { "Recorder",               FALSE, FALSE, FALSE, { NULL }, { "Audio", "Video", "AudioVideo", NULL } },
  { "DiscBurning",            FALSE, FALSE, FALSE, { NULL }, { "Audio", "Video", "AudioVideo", NULL } },
  { "ActionGame",             FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "AdventureGame",          FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "ArcadeGame",             FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "BoardGame",              FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "BlocksGame",             FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "CardGame",               FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "KidsGame",               FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "LogicGame",              FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "RolePlaying",            FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "Shooter",                FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "Simulation",             FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "SportsGame",             FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "StrategyGame",           FALSE, FALSE, FALSE, { NULL }, { "Game", NULL } },
  { "Art",                    FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Construction",           FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Music",                  FALSE, FALSE, FALSE, { NULL }, { "AudioVideo", "Education", NULL } },
  { "Languages",              FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "ArtificialIntelligence", FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Astronomy",              FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Biology",                FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Chemistry",              FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "ComputerScience",        FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "DataVisualization",      FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Economy",                FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Electricity",            FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Geography",              FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Geology",                FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Geoscience",             FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "History",                FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Humanities",             FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "ImageProcessing",        FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Literature",             FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Maps",                   FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", "Utility", NULL } },
  { "Math",                   FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "NumericalAnalysis",      FALSE, FALSE, FALSE, { NULL }, { "Education;Math", "Science;Math", NULL } },
  { "MedicalSoftware",        FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Physics",                FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Robotics",               FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "Spirituality",           FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", "Utility", NULL } },
  { "Sports",                 FALSE, FALSE, FALSE, { NULL }, { "Education", "Science", NULL } },
  { "ParallelComputing",      FALSE, FALSE, FALSE, { NULL }, { "Education;ComputerScience", "Science;ComputerScience", NULL } },
  { "Amusement",              FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "Archiving",              FALSE, FALSE, FALSE, { NULL }, { "Utility", NULL } },
  { "Compression",            FALSE, FALSE, FALSE, { NULL }, { "Utility;Archiving", NULL } },
  { "Electronics",            FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "Emulator",               FALSE, FALSE, FALSE, { NULL }, { "System", "Game", NULL } },
  { "Engineering",            FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "FileTools",              FALSE, FALSE, FALSE, { NULL }, { "Utility", "System", NULL } },
  { "FileManager",            FALSE, FALSE, FALSE, { NULL }, { "System;FileTools", NULL } },
  { "TerminalEmulator",       FALSE, FALSE, FALSE, { NULL }, { "System", NULL } },
  { "Filesystem",             FALSE, FALSE, FALSE, { NULL }, { "System", NULL } },
  { "Monitor",                FALSE, FALSE, FALSE, { NULL }, { "System", "Network", NULL } },
  { "Security",               FALSE, FALSE, FALSE, { NULL }, { "Settings", "System", NULL } },
  { "Accessibility",          FALSE, FALSE, FALSE, { NULL }, { "Settings", "Utility", NULL } },
  { "Calculator",             FALSE, FALSE, FALSE, { NULL }, { "Utility", NULL } },
  { "Clock",                  FALSE, FALSE, FALSE, { NULL }, { "Utility", NULL } },
  { "TextEditor",             FALSE, FALSE, FALSE, { NULL }, { "Utility", NULL } },
  { "Documentation",          FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "Adult",                  FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "Core",                   FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "COSMIC",                 FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "KDE",                    FALSE, FALSE, FALSE, { NULL }, { "Qt", NULL } },
  { "GNOME",                  FALSE, FALSE, FALSE, { NULL }, { "GTK", NULL } },
  { "XFCE",                   FALSE, FALSE, FALSE, { NULL }, { "GTK", NULL } },
  { "DDE",                    FALSE, FALSE, FALSE, { NULL }, { "Qt", NULL } },
  { "LXQt",                   FALSE, FALSE, FALSE, { NULL }, { "Qt", NULL } },
  { "GTK",                    FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "Qt",                     FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "Motif",                  FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "Java",                   FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "ConsoleOnly",            FALSE, FALSE, FALSE, { NULL }, { NULL } },
  { "Screensaver",            FALSE, TRUE,  FALSE, { NULL }, { NULL } },
  { "TrayIcon",               FALSE, TRUE,  FALSE, { NULL }, { NULL } },
  { "Applet",                 FALSE, TRUE,  FALSE, { NULL }, { NULL } },
  { "Shell",                  FALSE, TRUE,  FALSE, { NULL }, { NULL } },
  { "Application",            FALSE, FALSE, TRUE,  { NULL }, { NULL } },
  { "Applications",           FALSE, FALSE, TRUE,  { NULL }, { NULL } }
};

/* Escape values for console colors */
#define UNDERLINE     "\033[4m"
#define MAGENTA       "\033[35m"
#define RED           "\033[31m"
#define YELLOW        "\033[33m"

/* Colour definitions */
#define RESET_COLOR        (kf->use_colors ? "\033[0m" : "")
#define FILENAME_COLOR     (kf->use_colors ? UNDERLINE : "")
#define FATAL_COLOR        (kf->use_colors ? RED : "")
#define FUTURE_FATAL_COLOR (kf->use_colors ? RED : "")
#define WARNING_COLOR      (kf->use_colors ? MAGENTA : "")
#define HINT_COLOR         (kf->use_colors ? YELLOW : "")

G_GNUC_PRINTF (2, 3) static void
print_fatal (kf_validator *kf, const char *format, ...)
{
  va_list args;
  gchar *str;

  g_return_if_fail (kf != NULL && format != NULL);

  kf->fatal_error = TRUE;

  va_start (args, format);
  str = g_strdup_vprintf (format, args);
  va_end (args);

  g_print ("%s%s%s: %serror%s: %s",
           FILENAME_COLOR, kf->filename, RESET_COLOR,
           FATAL_COLOR, RESET_COLOR, str);

  g_free (str);
}

G_GNUC_PRINTF (2, 3) static void
print_future_fatal (kf_validator *kf, const char *format, ...)
{
  va_list args;
  gchar *str;

  g_return_if_fail (kf != NULL && format != NULL);

  va_start (args, format);
  str = g_strdup_vprintf (format, args);
  va_end (args);

  g_print ("%s%s%s: %serror%s: (will be fatal in the future): %s",
           FILENAME_COLOR, kf->filename, RESET_COLOR,
           FUTURE_FATAL_COLOR, RESET_COLOR, str);

  g_free (str);
}

G_GNUC_PRINTF (2, 3) static void
print_warning (kf_validator *kf, const char *format, ...)
{
  va_list args;
  gchar *str;

  g_return_if_fail (kf != NULL && format != NULL);

  va_start (args, format);
  str = g_strdup_vprintf (format, args);
  va_end (args);

  g_print ("%s%s%s: %swarning%s: %s",
           FILENAME_COLOR, kf->filename, RESET_COLOR,
           WARNING_COLOR, RESET_COLOR, str);

  g_free (str);
}

G_GNUC_PRINTF (2, 3) static void
print_hint (kf_validator *kf, const char *format, ...)
{
  va_list args;
  gchar *str;

  g_return_if_fail (kf != NULL && format != NULL);

  if (kf->no_hints)
    return;

  va_start (args, format);
  str = g_strdup_vprintf (format, args);
  va_end (args);

  g_print ("%s%s%s: %shint%s: %s",
           FILENAME_COLOR, kf->filename, RESET_COLOR,
           HINT_COLOR, RESET_COLOR, str);

  g_free (str);
}

/* + Key names must contain only the characters A-Za-z0-9-.
 *   Checked.
 */
static gboolean
key_is_valid (const char *key,
              int         len)
{
  char c;
  int i;

  for (i = 0; i < len; i++) {
    c = key[i];
    if (!g_ascii_isalnum (c) && c != '-')
      return FALSE;
  }

  return TRUE;
}

/* + Values of type string may contain all ASCII characters except for control
 *   characters.
 *   Checked.
 */
static gboolean
validate_string_key (kf_validator *kf,
                     const char   *key,
                     const char   *locale,
                     const char   *value)
{
  int      i;
  gboolean error;

  error = FALSE;

  for (i = 0; value[i] != '\0'; i++) {
    if (g_ascii_iscntrl (value[i])) {
      error = TRUE;
      break;
    }
  }

  if (error) {
    print_fatal (kf, "value \"%s\" for string key \"%s\" in group \"%s\" "
                     "contains invalid characters, string values may contain "
                     "all ASCII characters except for control characters\n",
                     value, key, kf->current_group);

    return FALSE;
  }

  return TRUE;
}

/* + Values of type localestring are user displayable, and are encoded in
 *   UTF-8.
 *   Checked.
 * + If a postfixed key occurs, the same key must be also present without the
 *   postfix.
 *   Checked.
 */
static gboolean
validate_localestring_key (kf_validator *kf,
                           const char   *key,
                           const char   *locale,
                           const char   *value)
{
  char *locale_key;

  if (locale)
    locale_key = g_strdup_printf ("%s[%s]", key, locale);
  else
    locale_key = g_strdup_printf ("%s", key);

  if (!g_utf8_validate (value, -1, NULL)) {
    print_fatal (kf, "value \"%s\" for locale string key \"%s\" in group "
                     "\"%s\" contains invalid UTF-8 characters, locale string "
                     "values should be encoded in UTF-8\n",
                     value, locale_key, kf->current_group);
    g_free (locale_key);

    return FALSE;
  }

  if (!g_hash_table_lookup (kf->current_keys, key)) {
    print_fatal (kf, "key \"%s\" in group \"%s\" is a localized key, but "
                     "there is no non-localized key \"%s\"\n",
                     locale_key, kf->current_group, key);
    g_free (locale_key);

    return FALSE;
  }

  g_free (locale_key);

  return TRUE;
}

/* + Values of type boolean must either be the string true or false.
 *   Checked.
 * + Historically some booleans have been represented by the numeric entries 0
 *   or 1. With this version of the standard they are now to be represented as
 *   a boolean string. However, if an implementation is reading a pre-1.0
 *   desktop entry, it should interpret 0 and 1 as false and true,
 *   respectively.
 *   Checked.
 */
static gboolean
validate_boolean_key (kf_validator *kf,
                      const char   *key,
                      const char   *locale,
                      const char   *value)
{
  if (strcmp (value, "true") && strcmp (value, "false") &&
      strcmp (value, "0")    && strcmp (value, "1")) {
    print_fatal (kf, "value \"%s\" for boolean key \"%s\" in group \"%s\" "
                     "contains invalid characters, boolean values must be "
                     "\"false\" or \"true\"\n",
                     value, key, kf->current_group);
    return FALSE;
  }

  if (!kf->no_deprecated_warnings &&
      (!strcmp (value, "0") || !strcmp (value, "1")))
    print_warning (kf, "boolean key \"%s\" in group \"%s\" has value \"%s\", "
                       "which is deprecated: boolean values should be "
                       "\"false\" or \"true\"\n",
                       key, kf->current_group, value);

  return TRUE;
}

/* + Values of type numeric must be a valid floating point number as recognized
 *   by the %f specifier for scanf.
 *   Checked.
 */
static gboolean
validate_numeric_key (kf_validator *kf,
                      const char   *key,
                      const char   *locale,
                      const char   *value)
{
  float d;
  int res;

  res = sscanf (value, "%f", &d);
  if (res == 0) {
    print_fatal (kf, "value \"%s\" for numeric key \"%s\" in group \"%s\" "
                     "contains invalid characters, numeric values must be "
                     "valid floating point numbers\n",
                     value, key, kf->current_group);
    return FALSE;
  }

  return TRUE;
}

/* + Values of type string may contain all ASCII characters except for control
 *   characters.
 *   Checked.
 * + FIXME: how should an empty list be handled?
 */
static gboolean
validate_string_regexp_list_key (kf_validator *kf,
                                 const char   *key,
                                 const char   *locale,
                                 const char   *value,
                                 const char   *type)
{
  int      i;
  gboolean error;

  error = FALSE;

  for (i = 0; value[i] != '\0'; i++) {
    if (g_ascii_iscntrl (value[i])) {
      error = TRUE;
      break;
    }
  }

  if (error) {
    print_fatal (kf, "value \"%s\" for %s list key \"%s\" in group \"%s\" "
                     "contains invalid character '%c', %s list values may "
                     "contain all ASCII characters except for control "
                     "characters\n",
                     value, type, key, kf->current_group, value[i], type);

    return FALSE;
  }

  return TRUE;
}

static gboolean
validate_string_list_key (kf_validator *kf,
                          const char   *key,
                          const char   *locale,
                          const char   *value)
{
  return validate_string_regexp_list_key (kf, key, locale, value, "string");
}

static gboolean
validate_regexp_list_key (kf_validator *kf,
                          const char   *key,
                          const char   *locale,
                          const char   *value)
{
  return validate_string_regexp_list_key (kf, key, locale, value, "regexp");
}

/* + Values of type localestring are user displayable, and are encoded in
 *   UTF-8.
 *   FIXME: partly checked; we checked the whole value is encored in UTF-8, but
 *   not that each value of the list is. Although this might be equivalent?
 * + If a postfixed key occurs, the same key must be also present without the
 *   postfix.
 *   Checked.
 * + FIXME: how should an empty list be handled?
 */
static gboolean
validate_localestring_list_key (kf_validator *kf,
                                const char   *key,
                                const char   *locale,
                                const char   *value)
{
  char     *locale_key;

  if (locale)
    locale_key = g_strdup_printf ("%s[%s]", key, locale);
  else
    locale_key = g_strdup_printf ("%s", key);


  if (!g_utf8_validate (value, -1, NULL)) {
    print_fatal (kf, "value \"%s\" for locale string list key \"%s\" in group "
                     "\"%s\" contains invalid UTF-8 characters, locale string "
                     "list values should be encoded in UTF-8\n",
                     value, locale_key, kf->current_group);
    g_free (locale_key);

    return FALSE;
  }

  if (!g_hash_table_lookup (kf->current_keys, key)) {
    print_fatal (kf, "key \"%s\" in group \"%s\" is a localized key, but "
                     "there is no non-localized key \"%s\"\n",
                     locale_key, kf->current_group, key);
    g_free (locale_key);

    return FALSE;
  }

  g_free (locale_key);

  return TRUE;
}

/* + This specification defines 3 types of desktop entries: Application
 *   (type 1), Link (type 2) and Directory (type 3). To allow the addition of
 *   new types in the future, implementations should ignore desktop entries
 *   with an unknown type.
 *   Checked.
 * + KDE specific types: ServiceType, Service and FSDevice
 *   Checked.
 */
static gboolean
handle_type_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value)
{
  unsigned int i;

  for (i = 0; i < G_N_ELEMENTS (registered_types); i++) {
    if (!strcmp (value, registered_types[i].name))
      break;
  }

  if (i == G_N_ELEMENTS (registered_types)) {
    /* force the type, since the key might be present multiple times... */
    kf->type = INVALID_TYPE;

    print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                     "is not a registered type value (\"Application\", "
                     "\"Link\" and \"Directory\")\n",
                     value, locale_key, kf->current_group);
    return FALSE;
  }

  if (registered_types[i].kde_reserved && kf->kde_reserved_warnings)
    print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "is a reserved value for KDE\n",
                       value, locale_key, kf->current_group);

  if (registered_types[i].deprecated && !kf->no_deprecated_warnings)
    print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "is deprecated\n",
                       value, locale_key, kf->current_group);

  kf->type = registered_types[i].type;
  kf->type_string = registered_types[i].name;

  return TRUE;
}

/* + Entries that confirm with this version of the specification should use
 *   1.0.
 *   Checked.
 * + Previous versions of the spec: 0.9.x where 3 <= x <= 8
 *   Checked.
 */
static gboolean
handle_version_key (kf_validator *kf,
                    const char   *locale_key,
                    const char   *value)
{
  if (!strcmp (value, "1.5"))
    return TRUE;

  if (!strcmp (value, "1.4"))
    return TRUE;

  if (!strcmp (value, "1.3"))
    return TRUE;

  if (!strcmp (value, "1.2"))
    return TRUE;

  if (!strcmp (value, "1.1"))
    return TRUE;

  if (!strcmp (value, "1.0"))
    return TRUE;

  if (!strncmp (value, "0.9.", strlen ("0.9."))) {
    char c;

    c = value[strlen ("0.9.")];
    if ('3' <= c && c <= '8' && value[strlen ("0.9.") + 1] == '\0')
      return TRUE;
  }

  print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                   "is not a known version\n",
                   value, locale_key, kf->current_group);
  return FALSE;
}

/* + Tooltip for the entry, for example "View sites on the Internet", should
 *   not be redundant with Name or GenericName.
 *   Checked.
 */
static gboolean
handle_comment_key (kf_validator *kf,
                    const char   *locale_key,
                    const char   *value)
{
  char        *locale_compare_key;
  kf_keyvalue *keyvalue;

  locale_compare_key = g_strdup_printf ("Name%s",
                                        locale_key + strlen ("Comment"));
  keyvalue = g_hash_table_lookup (kf->current_keys, locale_compare_key);
  g_free (locale_compare_key);

  if (keyvalue && g_ascii_strcasecmp (value, keyvalue->value) == 0) {
    print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "looks the same as that of key \"%s\"\n",
                       value, locale_key, kf->current_group,
                       keyvalue->key);
    return FALSE;
  }

  locale_compare_key = g_strdup_printf ("GenericName%s",
                                        locale_key + strlen ("Comment"));
  keyvalue = g_hash_table_lookup (kf->current_keys, locale_compare_key);
  g_free (locale_compare_key);

  if (keyvalue && g_ascii_strcasecmp (value, keyvalue->value) == 0) {
    print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "looks the same as that of key \"%s\"\n",
                       value, locale_key, kf->current_group,
                       keyvalue->key);
    return FALSE;
  }

  return TRUE;
}

/* + If the name is an absolute path, the given file will be used.
 *   Checked.
 * + If the name is not an absolute path, the algorithm described in the Icon
 *   Theme Specification will be used to locate the icon.
 *   Checked.
 *   FIXME: add clarification to desktop entry spec that the name doesn't
 *   contain an extension
 */
static gboolean
handle_icon_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value)
{
  if (g_path_is_absolute (value)) {
    if (g_str_has_suffix (value, "/")) {
      print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" is an "
                       "absolute path to a directory, instead of being an "
                       "absolute path to an icon or an icon name\n",
                       value, locale_key, kf->current_group);
      return FALSE;
    } else
      return TRUE;
  }

  if (g_utf8_strchr (value, -1, G_DIR_SEPARATOR)) {
    print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" looks like "
                     "a relative path, instead of being an absolute path to "
                     "an icon or an icon name\n",
                     value, locale_key, kf->current_group);
    return FALSE;
  }

  if (g_str_has_suffix (value, ".png") ||
      g_str_has_suffix (value, ".xpm") ||
      g_str_has_suffix (value, ".svg")) {
    print_future_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" is an "
                            "icon name with an extension, but there should be "
                            "no extension as described in the Icon Theme "
                            "Specification if the value is not an absolute "
                            "path\n",
                            value, locale_key, kf->current_group);
    return FALSE;
  }

  return TRUE;
}

/* + Only one of these keys, either OnlyShowIn or NotShowIn, may appear in a
 *   group.
 *   Checked.
 * + (for possible values see the Desktop Menu Specification)
 *   Checked.
 *   FIXME: this is not perfect because it could fail if a new value with
 *   a semicolon is registered.
 * + All values extending the format should start with "X-".
 *   Checked.
 * + FIXME: is this okay to have only ";"? (gnome-theme-installer.desktop does)
 */
static gboolean
handle_show_in_key (kf_validator *kf,
                    const char   *locale_key,
                    const char   *value)
{
  gboolean       retval;
  char         **show;
  GHashTable    *hashtable;
  int            i;
  unsigned int   j;

  retval = TRUE;

  if (kf->show_in) {
    print_fatal (kf, "only one of \"OnlyShowIn\" and \"NotShowIn\" keys "
                     "may appear in group \"%s\"\n",
                     kf->current_group);
    retval = FALSE;
  }
  kf->show_in = TRUE;

  hashtable = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
  show = g_strsplit (value, ";", 0);

  for (i = 0; show[i]; i++) {
    /* since the value ends with a semicolon, we'll have an empty string
     * at the end */
    if (*show[i] == '\0' && show[i + 1] == NULL)
      break;

    if (g_hash_table_lookup (hashtable, show[i])) {
      print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "contains \"%s\" more than once\n",
                         value, locale_key, kf->current_group, show[i]);
      continue;
    }

    g_hash_table_insert (hashtable, show[i], show[i]);

    if (!strncmp (show[i], "X-", 2))
      continue;

    for (j = 0; j < G_N_ELEMENTS (show_in_registered); j++) {
      if (!strcmp (show[i], show_in_registered[j]))
        break;
    }

    if (j == G_N_ELEMENTS (show_in_registered)) {
      print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "contains an unregistered value \"%s\"; values "
                       "extending the format should start with \"X-\"\n",
                       value, locale_key, kf->current_group, show[i]);
      retval = FALSE;
    }
  }

  g_strfreev (show);
  g_hash_table_destroy (hashtable);

  return retval;
}

/* + A command line consists of an executable program optionally followed by
 *   one or more arguments. The executable program can either be specified with
 *   its full path or with the name of the executable only. If no full path is
 *   provided the executable is looked up in the $PATH used by the desktop
 *   environment. The name or path of the executable program may not contain
 *   the equal sign ("=").
 *   FIXME
 * + Arguments are separated by a space.
 *   FIXME
 * + Arguments may be quoted in whole.
 *   FIXME
 * + If an argument contains a reserved character the argument must be quoted.
 *   Checked.
 * + The rules for quoting of arguments is also applicable to the executable
 *   name or path of the executable program as provided.
 *   FIXME
 * + Quoting must be done by enclosing the argument between double quotes and
 *   escaping the double quote character, backtick character ("`"), dollar sign
 *   ("$") and backslash character ("\") by preceding it with an additional
 *   backslash character. Implementations must undo quoting before expanding
 *   field codes and before passing the argument to the executable program.
 *   Reserved characters are space (" "), tab, newline, double quote, single
 *   quote ("'"), backslash character ("\"), greater-than sign (">"), less-than
 *   sign ("<"), tilde ("~"), vertical bar ("|"), ampersand ("&"), semicolon
 *   (";"), dollar sign ("$"), asterisk ("*"), question mark ("?"), hash mark
 *   ("#"), parenthesis ("(") and (")") and backtick character ("`").
 *   Checked.
 * + Note that the general escape rule for values of type string states that
 *   the backslash character can be escaped as ("\\") as well and that this
 *   escape rule is applied before the quoting rule. As such, to unambiguously
 *   represent a literal backslash character in a quoted argument in a desktop
 *   entry file requires the use of four successive backslash characters
 *   ("\\\\"). Likewise, a literal dollar sign in a quoted argument in a
 *   desktop entry file is unambiguously represented with ("\\$").
 *   Checked.
 * + Field codes consist of the percentage character ("%") followed by an alpha
 *   character. Literal percentage characters must be escaped as %%.
 *   Checked.
 * + Command lines that contain a field code that is not listed in this
 *   specification are invalid and must not be processed, in particular
 *   implementations may not introduce support for field codes not listed in
 *   this specification. Extensions, if any, should be introduced by means of a
 *   new key.
 *   Checked.
 * + A command line may contain at most one %f, %u, %F or %U field code.
 *   Checked.
 * + The %F and %U field codes may only be used as an argument on their own.
 *   FIXME
 */
static gboolean
handle_exec_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value)
{
  gboolean    retval;
  gboolean    file_uri;
  gboolean    in_quote;
  gboolean    escaped;
  gboolean    flag;
  const char *c;

  retval = TRUE;

  file_uri = FALSE;
  in_quote = FALSE;
  escaped  = FALSE;
  flag     = FALSE;

#define PRINT_INVALID_IF_FLAG                                       \
  if (flag) {                                                       \
    print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" " \
                     "contains an invalid field code \"%%%c\"\n",   \
                     value, locale_key, kf->current_group, *c);     \
    retval = FALSE;                                                 \
    flag = FALSE;                                                   \
    break;                                                          \
  }

  c = value;
  while (*c) {
    switch (*c) {
      /* quotes and escaped characters in quotes */
      case '"':
        PRINT_INVALID_IF_FLAG;
        if (in_quote) {
          if (!escaped)
            in_quote = FALSE;
        } else {
          if (!escaped)
            in_quote = TRUE;
          else {
            print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                             "contains an escaped double quote (\\\\\") "
                             "outside of a quote, but the double quote is "
                             "a reserved character\n",
                             value, locale_key, kf->current_group);
            retval = FALSE;
          }
        }

        escaped = FALSE;
        break;
      case '`':
      case '$':
        PRINT_INVALID_IF_FLAG;
        if (in_quote) {
          if (!escaped) {
            print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                             "contains a non-escaped character '%c' in a "
                             "quote, but it should be escaped with two "
                             "backslashes (\"\\\\%c\")\n",
                             value, locale_key, kf->current_group, *c, *c);
            retval = FALSE;
          } else
            escaped = FALSE;
        } else {
          print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                           "contains a reserved character '%c' outside of a "
                           "quote\n",
                           value, locale_key, kf->current_group, *c);
          retval = FALSE;
        }
        break;
      case '\\':
        PRINT_INVALID_IF_FLAG;

        /* Escape character immediately followed by \0? */
        if (*(c + 1) == '\0') {
          print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                           "ends in an incomplete escape sequence\n",
                           value, locale_key, kf->current_group);
          retval = FALSE;
          break;
        }

        c++;
        if (*c == '\\' && in_quote)
          escaped = !escaped;
        break;

      /* reserved characters */
      case ' ':
        //FIXME
        break;
      case '\t':
      case '\n':
      case '\'':
      case '>':
      case '<':
      case '~':
      case '|':
      case '&':
      case ';':
      case '*':
      case '?':
      case '#':
      case '(':
      case ')':
        PRINT_INVALID_IF_FLAG;
        if (!in_quote) {
          print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                           "contains a reserved character '%c' outside of a "
                           "quote\n",
                           value, locale_key, kf->current_group, *c);
          retval = FALSE;
        }
        break;

      /* flags */
      case '%':
        flag = !flag;
        break;
      case 'f':
      case 'u':
        if (flag) {
          if (file_uri) {
            print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                             "may contain at most one \"%%f\", \"%%u\", "
                             "\"%%F\" or \"%%U\" field code\n",
                             value, locale_key, kf->current_group);
            retval = FALSE;
          }

          file_uri = TRUE;
          flag = FALSE;
        }
        break;
      case 'F':
      case 'U':
        if (flag) {
          if (file_uri) {
            print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                             "may contain at most one \"%%f\", \"%%u\", "
                             "\"%%F\" or \"%%U\" field code\n",
                             value, locale_key, kf->current_group);
            retval = FALSE;
          }

          file_uri = TRUE;
          flag = FALSE;
        }
        break;
      case 'i':
      case 'c':
      case 'k':
        if (flag)
          flag = FALSE;
        break;
      case 'd':
      case 'D':
      case 'n':
      case 'N':
      case 'v':
      case 'm':
        if (flag) {
          if (!kf->no_deprecated_warnings)
            print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                               "contains a deprecated field code \"%%%c\"\n",
                                value, locale_key, kf->current_group, *c);
          flag = FALSE;
        }
        break;

      default:
        PRINT_INVALID_IF_FLAG;
        break;
    }

    c++;
  }

  if (in_quote) {
    print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" contains a "
                     "quote which is not closed\n",
                     value, locale_key, kf->current_group);
    retval = FALSE;
  }

  if (flag) {
    print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" contains a "
                     "non-complete field code\n",
                     value, locale_key, kf->current_group);
    retval = FALSE;
  }

  return retval;
}

/* See checks for handle_exec_key().
 */
static gboolean
handle_desktop_exec_key (kf_validator *kf,
                         const char   *locale_key,
                         const char   *value)
{
  handle_key_for_application (kf, locale_key, value);

  return handle_exec_key (kf, locale_key, value);
}

/* + If entry is of type Application, the working directory to run the program
 *   in. (probably implies an absolute path)
 *   Checked.
 * + FIXME: is it okay to have an empty string here? (wireshark.desktop does)
 */
static gboolean
handle_path_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value)
{
  handle_key_for_application (kf, locale_key, value);

  if (!g_path_is_absolute (value))
    print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "does not look like an absolute path\n",
                       value, locale_key, kf->current_group);

  return TRUE;
}

/* + The MIME type(s) supported by this application. Check they are valid
 *   MIME types.
 *   Checked.
 */
static gboolean
handle_mime_key (kf_validator *kf,
                 const char   *locale_key,
                 const char   *value)
{
  gboolean       retval;
  char         **types;
  GHashTable    *hashtable;
  int            i;
  char          *valid_error;
  MimeUtilsValidity valid;

  handle_key_for_application (kf, locale_key, value);

  retval = TRUE;

  hashtable = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
  types = g_strsplit (value, ";", 0);

  for (i = 0; types[i]; i++) {
    /* since the value ends with a semicolon, we'll have an empty string
     * at the end */
    if (*types[i] == '\0' && types[i + 1] == NULL)
      break;

    if (g_hash_table_lookup (hashtable, types[i])) {
      print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "contains \"%s\" more than once\n",
                         value, locale_key, kf->current_group, types[i]);
      continue;
    }

    g_hash_table_insert (hashtable, types[i], types[i]);

    valid = mu_mime_type_is_valid (types[i], &valid_error);
    switch (valid) {
      case MU_VALID:
        break;
      case MU_DISCOURAGED:
        print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                           "contains value \"%s\" which is a MIME type that "
                           "should probably not be used: %s\n",
                           value, locale_key, kf->current_group,
                           types[i], valid_error);

        g_free (valid_error);
        break;
      case MU_INVALID:
        print_future_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                                "contains value \"%s\" which is an invalid "
                                "MIME type: %s\n",
                                value, locale_key, kf->current_group,
                                types[i], valid_error);

        retval = FALSE;
        g_free (valid_error);
        break;
      default:
        g_assert_not_reached ();
    }
  }

  g_strfreev (types);
  g_hash_table_destroy (hashtable);

  return retval;
}

/* + FIXME: are there restrictions on how a category should be named?
 * + Categories in which the entry should be shown in a menu (for possible
 *   values see the Desktop Menu Specification).
 *   Checked.
 * + The table below describes Reserved Categories. Reserved Categories have a
 *   specific desktop specific meaning that has not been standardized (yet).
 *   Desktop entry files that use a reserved category MUST also include an
 *   appropriate OnlyShowIn= entry to restrict themselves to those environments
 *   that properly support the reserved category as used.
 *   Checked.
 * + Accept "Application" as a deprecated category.
 *   Checked.
 *   FIXME: it's not really deprecated, so the error message is wrong
 * + All categories extending the format should start with "X-".
 *   Checked.
 * + Using multiple main categories may lead to appearing more than once in
 *   application menu.
 *   Checked.
 * + One main category should be included, otherwise application will appear in
 *   "catch-all" section of application menu.
 *   Checked.
 *   FIXME: decide if it's okay to have an empty list of categories.
 * + Some categories, if included, require that another category is included.
 *   Eg: if Audio is there, AudioVideo must be there.
 *   Checked.
 * + Some categories, if included, suggest that another category is included.
 *   Eg: Debugger suggests Development.
 *   This is the case for most additional categories.
 *   Checked.
 */
static gboolean
handle_categories_key (kf_validator *kf,
                       const char   *locale_key,
                       const char   *value)
{
  gboolean       retval;
  char         **categories;
  GHashTable    *hashtable;
  int            i;
  unsigned int   j;
  int            main_categories_nb;

  handle_key_for_application (kf, locale_key, value);

  retval = TRUE;

  /* accept empty value as valid: this is like having no category at all */
  if (value[0] == '\0')
    return retval;

  hashtable = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
  categories = g_strsplit (value, ";", 0);

  /* this is a two-pass check: we first put the categories in a hash table so
   * that they are easy-to-find, and we then do many checks */

  /* first pass */
  for (i = 0; categories[i]; i++) {
    /* since the value ends with a semicolon, we'll have an empty string
     * at the end */
    if (*categories[i] == '\0' && categories[i + 1] == NULL)
      break;

    if (g_hash_table_lookup (hashtable, categories[i])) {
      print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "contains \"%s\" more than once\n",
                         value, locale_key, kf->current_group, categories[i]);
      continue;
    }

    g_hash_table_insert (hashtable, categories[i], categories[i]);
  }

  /* second pass */
  main_categories_nb = 0;

  for (i = 0; categories[i]; i++) {
    unsigned int k;

    /* since the value ends with a semicolon, we'll have an empty string
     * at the end */
    if (*categories[i] == '\0' && categories[i + 1] == NULL)
      break;

    if (!strncmp (categories[i], "X-", 2))
      continue;

    for (j = 0; j < G_N_ELEMENTS (registered_categories); j++) {
      if (!strcmp (categories[i], registered_categories[j].name))
        break;
    }

    if (j == G_N_ELEMENTS (registered_categories)) {
      print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "contains an unregistered value \"%s\"; values "
                       "extending the format should start with \"X-\"\n",
                       value, locale_key, kf->current_group, categories[i]);
      retval = FALSE;
      continue;
    }

    if (registered_categories[j].main) {
      /* only count it as a main category if none of the required categories
       * for this one is also a main category (and is present) */
      gboolean required_main_category_present = FALSE;

      for (k = 0; registered_categories[j].requires[k] != NULL; k++) {
        char **required_categories;
        int    l;

        required_categories = g_strsplit (registered_categories[j].requires[k],
                                          ";", 0);

        for (l = 0; required_categories[l]; l++) {
          unsigned int m;

          if (!g_hash_table_lookup (hashtable, required_categories[l]))
            continue;

          for (m = 0; m < G_N_ELEMENTS (registered_categories); m++) {
            if (strcmp (required_categories[l],
                        registered_categories[m].name) != 0)
              continue;

            if (registered_categories[m].main)
              required_main_category_present = TRUE;

            break;
          }

          if (required_main_category_present)
            break;
        }

        if (required_main_category_present) {
          g_strfreev (required_categories);
          break;
        }

        g_strfreev (required_categories);
      }

      if (!required_main_category_present)
        main_categories_nb++;
    }

    if (registered_categories[j].main && main_categories_nb > 1)
      print_hint (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                      "contains more than one main category; application "
                      "might appear more than once in the application menu\n",
                      value, locale_key, kf->current_group);


    if (registered_categories[j].deprecated) {
      if (!kf->no_deprecated_warnings)
        print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                           "contains a deprecated value \"%s\"\n",
                            value, locale_key, kf->current_group,
                            categories[i]);
    }

    if (registered_categories[j].require_only_show_in) {
      if (!g_hash_table_lookup (kf->current_keys, "OnlyShowIn")) {
        print_fatal (kf, "value item \"%s\" in key \"%s\" in group \"%s\" "
                         "is a reserved category, so a \"OnlyShowIn\" key "
                         "must be included\n",
                         categories[i], locale_key, kf->current_group);
        retval = FALSE;
      }
    }

    /* required categories */

    for (k = 0; registered_categories[j].requires[k] != NULL; k++) {
      char **required_categories;
      int    l;

      required_categories = g_strsplit (registered_categories[j].requires[k],
                                        ";", 0);

      for (l = 0; required_categories[l]; l++) {
        if (!g_hash_table_lookup (hashtable, required_categories[l]))
          break;
      }

      /* we've reached the end of a list of required categories, so
       * the condition is satisfied */
      if (required_categories[l] == NULL) {
        g_strfreev (required_categories);
        break;
      }

      g_strfreev (required_categories);
    }

    /* we've reached the end of a non-empty set of required categories; this
     * means none of the possible required category (or list of required
     * categories) was found */
    if (k != 0 && registered_categories[j].requires[k] == NULL) {
      GString *output_required;

      output_required = g_string_new (registered_categories[j].requires[0]);
      for (k = 1; registered_categories[j].requires[k] != NULL; k++)
        g_string_append_printf (output_required, ", or %s",
                                registered_categories[j].requires[k]);

      print_future_fatal (kf, "value item \"%s\" in key \"%s\" in group \"%s\" "
                              "requires another category to be present among "
                              "the following categories: %s\n",
                              categories[i], locale_key, kf->current_group,
                              output_required->str);

      g_string_free (output_required, TRUE);
      retval = FALSE;
    }

    /* suggested categories */

    for (k = 0; registered_categories[j].suggests[k] != NULL; k++) {
      char **suggested_categories;
      int    l;

      suggested_categories = g_strsplit (registered_categories[j].suggests[k],
                                         ";", 0);

      for (l = 0; suggested_categories[l]; l++) {
        if (!g_hash_table_lookup (hashtable, suggested_categories[l]))
          break;
      }

      /* we've reached the end of a list of suggested categories, so
       * the condition is satisfied */
      if (suggested_categories[l] == NULL) {
        g_strfreev (suggested_categories);
        break;
      }

      g_strfreev (suggested_categories);
    }

    /* we've reached the end of a non-empty set of suggested categories; this
     * means none of the possible suggested category (or list of suggested
     * categories) was found */
    if (k != 0 && registered_categories[j].suggests[k] == NULL) {
      GString *output_suggested;

      output_suggested = g_string_new (registered_categories[j].suggests[0]);
      for (k = 1; registered_categories[j].suggests[k] != NULL; k++)
        g_string_append_printf (output_suggested, ", or %s",
                                registered_categories[j].suggests[k]);

      print_hint (kf, "value item \"%s\" in key \"%s\" in group \"%s\" "
                      "can be extended with another category among the "
                      "following categories: %s\n",
                      categories[i], locale_key, kf->current_group,
                      output_suggested->str);

      g_string_free (output_suggested, TRUE);
    }

  }

  g_strfreev (categories);
  g_hash_table_destroy (hashtable);

  g_assert (main_categories_nb >= 0);

  if (main_categories_nb == 0)
    print_hint (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                    "does not contain a registered main category; application "
                    "might only show up in a \"catch-all\" section of the "
                    "application menu\n",
                    value, locale_key, kf->current_group);

  return retval;
}

/* + Identifiers for application actions. Check they are using a valid format.
 *   Checked.
 *
 * Note that we will check later on (in * validate_actions()) that there is a
 * "Desktop Action foobar" group for each "foobar" identifier.
 */
static gboolean
handle_actions_key (kf_validator *kf,
                    const char   *locale_key,
                    const char   *value)
{
  char **actions;
  char  *action;
  int    i;
  gboolean retval;

  handle_key_for_application (kf, locale_key, value);

  retval = TRUE;
  actions = g_strsplit (value, ";", 0);

  for (i = 0; actions[i]; i++) {
    /* since the value ends with a semicolon, we'll have an empty string
     * at the end */
    if (*actions[i] == '\0') {
      if (actions[i + 1] != NULL) {
        print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "contains an empty action\n",
                         value, locale_key, kf->current_group);
        retval = FALSE;
        break;
      }

      continue;
    }

    if (g_hash_table_lookup (kf->action_values, actions[i])) {
      print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "contains action \"%s\" more than once\n",
                         value, locale_key, kf->current_group, actions[i]);
      continue;
    }

    if (!key_is_valid (actions[i], strlen (actions[i]))) {
      print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "contains invalid action identifier \"%s\", only "
                       "alphanumeric characters and '-' are allowed\n",
                       value, locale_key, kf->current_group, actions[i]);
      retval = FALSE;
      break;
    }

    action = g_strdup (actions[i]);
    g_hash_table_insert (kf->action_values, action, action);
  }

  g_strfreev (actions);

  return retval;
}

/* + DBus interface names. Check they are using a valid format.
 *   Checked.
 *
 * Note that we will check later on (in * validate_actions()) that there is a
 * "Desktop Action foobar" group for each "foobar" identifier.
 */
static gboolean
handle_implements_key (kf_validator *kf,
                       const char   *locale_key,
                       const char   *value)
{
  char **interfaces;
  char  *interface;
  int    i;
  gboolean retval;

  retval = TRUE;
  interfaces = g_strsplit (value, ";", 0);

  for (i = 0; interfaces[i]; i++) {
    if (!g_dbus_is_interface_name (interfaces[i])) {
      print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "contains an invalid interface name \"%s\"\n",
                       value, locale_key, kf->current_group, interfaces[i]);
      retval = FALSE;
      break;
    }

    if (g_hash_table_lookup (kf->interfaces, interfaces[i])) {
      print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "contains interface \"%s\" more than once\n",
                         value, locale_key, kf->current_group, interfaces[i]);
      continue;
    }

    interface = g_strdup (interfaces[i]);
    g_hash_table_insert (kf->interfaces, interface, interface);
  }

  g_strfreev (interfaces);

  return retval;
}

/* + If the file describes a D-Bus activatable service, the filename must be in
 *   reverse-DNS notation, i.e. contain at least two dots including the dot
 *   in ".desktop".
 *   Checked.
 */
static gboolean
handle_dbus_activatable_key (kf_validator *kf,
                             const char   *locale_key,
                             const char   *value)
{
  gchar *basename_utf8;
  gchar *basename;
  const gchar *p = NULL;
  gboolean retval = TRUE;

  /* If DBusActivatable=false, don't check */
  if (strcmp (value, "true") && strcmp (value, "1"))
    return TRUE;

  basename = g_path_get_basename (kf->filename);
  basename_utf8 = g_filename_to_utf8 (basename, -1, NULL, NULL, NULL);
  if (!basename_utf8)
    goto out;

  p = g_utf8_strchr (basename_utf8, -1, '.');
  if (!p)
    goto out;
  p = g_utf8_strchr (p + 1, -1, '.');

out:
  if (!p) {
    print_fatal (kf, "DBusActivatable filename must conform to reverse-DNS notation\n");
    retval = FALSE;
  }

  g_free (basename_utf8);
  g_free (basename);
  return retval;
}

/* + The device to mount. (probably implies an absolute path)
 *   Checked.
 */
static gboolean
handle_dev_key (kf_validator *kf,
                const char   *locale_key,
                const char   *value)
{
  handle_key_for_fsdevice (kf, locale_key, value);

  if (!g_path_is_absolute (value))
    print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "does not look like an absolute path\n",
                       value, locale_key, kf->current_group);

  return TRUE;
}

/* + The mount point of the device in question. (probably implies an absolute
 *   path)
 *   Checked.
 */
static gboolean
handle_mountpoint_key (kf_validator *kf,
                       const char   *locale_key,
                       const char   *value)
{
  handle_key_for_fsdevice (kf, locale_key, value);

  if (!g_path_is_absolute (value))
    print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "does not look like an absolute path\n",
                       value, locale_key, kf->current_group);

  return TRUE;
}

/* + Possible values are UTF-8 and Legacy-Mixed.
 *   Checked.
 */
static gboolean
handle_encoding_key (kf_validator *kf,
                     const char   *locale_key,
                     const char   *value)
{
  if (!strcmp (value, "UTF-8") || !strcmp (value, "Legacy-Mixed"))
    return TRUE;

  print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                   "is not a registered encoding value (\"UTF-8\", and "
                   "\"Legacy-Mixed\")\n",
                   value, locale_key, kf->current_group);

  return FALSE;
}

/* + See http://lists.freedesktop.org/archives/xdg/2007-January/007436.html
 * + Value is one of:
 *   - if-exists FILE
 *   - unless-exists FILE
 *   - DESKTOP-ENVIRONMENT-NAME [DESKTOP-SPECIFIC-TEST]
 *   - other known conditions (GNOME3, GSettings, etc.)
 *   Checked.
 * + FILE must be a path to a filename, relative to $XDG_CONFIG_HOME.
 *   Checked.
 * + DESKTOP-ENVIRONMENT-NAME should be a registered value (in Desktop Menu
 *   Specification) or start with "X-".
 *   Checked.
 * + [DESKTOP-SPECIFIC-TEST] is optional.
 *   Checked.
 */
static gboolean
handle_autostart_condition_key (kf_validator *kf,
                                const char   *locale_key,
                                const char   *value)
{
  gboolean  retval;
  char     *condition;
  char     *argument;

  handle_key_for_application (kf, locale_key, value);

  retval = TRUE;

  condition = g_strdup (value);
  argument = g_utf8_strchr (condition, -1, ' ');

  if (argument) {
    /* make condition a 0-ended string */
    *argument = '\0';

    /* skip the space(s) */
    argument++;
    while (*argument == ' ') {
      argument++;
    }
  }

  if (!strcmp (condition, "if-exists") || !strcmp (condition, "unless-exists")) {
    if (!argument || argument[0] == '\0') {
      print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "does not contain a path to a file to test the "
                       "condition\n",
                       value, locale_key, kf->current_group);
      retval = FALSE;
    } else if (argument[0] == G_DIR_SEPARATOR) {
      print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                       "contains a path \"%s\" that is absolute, while it "
                       "should be relative (to $XDG_CONFIG_HOME)\n",
                       value, locale_key, kf->current_group, argument);
      retval = FALSE;
    } else if (argument[0] == '.' &&
               ((strlen (argument) == 2 &&
                 argument[1] == '.') ||
                (strlen (argument) >= 3 &&
                 argument[1] == '.' &&
                 argument[2] == G_DIR_SEPARATOR))) {
      print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "contains a path \"%s\" that depends on the value "
                         "of $XDG_CONFIG_HOME (\"..\" should be avoided)\n",
                         value, locale_key, kf->current_group, argument);
    }

  } else if (strncmp (condition, "X-", 2) == 0) {
    if (argument && argument[0] == '\0')
      print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "has trailing space(s)\n",
                         value, locale_key, kf->current_group);
  } else {
    unsigned int i;
    unsigned int j;

    /* Look if it's a registered AutostartCondition */

    for (i = 0; i < G_N_ELEMENTS (registered_autostart_condition); i++) {

      if (strcmp (condition, registered_autostart_condition[i].name) != 0)
        continue;

      /* check if first argument is one of the expected ones */
      for (j = 0; registered_autostart_condition[i].first_arg[j] != NULL; j++) {
        const char *first = registered_autostart_condition[i].first_arg[j];
        char       *after_first = argument;

        if (argument && !strncmp (argument, first, strlen (first))) {
          after_first += strlen (first);
          if (after_first[0] == '\0' || after_first[0] == ' ') {
            /* find next argument */
            argument = after_first;
            while (*argument == ' ')
              argument++;
          }

          break;
        }
      }

      /* we've reached the end of a non-empty set of first arguments; this
       * means none of the possible first arguments was found */
      if (j != 0 && registered_autostart_condition[i].first_arg[j] == NULL) {
        GString *output;

        output = g_string_new (registered_autostart_condition[i].first_arg[0]);
        for (j = 1; registered_autostart_condition[i].first_arg[j] != NULL; j++)
          g_string_append_printf (output, ", or %s",
                                  registered_autostart_condition[i].first_arg[j]);

        print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "does not contain a valid first argument for "
                         "condition \"%s\"; valid first arguments are: %s\n",
                         value, locale_key, kf->current_group,
                         condition, output->str);
        retval = FALSE;

        g_string_free (output, TRUE);

      } else {

        switch (registered_autostart_condition[i].additional_args) {
          case 0:
            if (argument && argument[0] != '\0') {
              print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                               "has too many arguments for condition \"%s\"\n",
                               value, locale_key, kf->current_group, condition);
              retval = FALSE;
            }
            break;

          case 1:
            /* we handle the "one argument" case specially, as spaces might be
             * normal there, and therefore we don't want to split the string
             * based on spaces */
            if (!argument || argument[0] == '\0') {
              print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                               "is missing a last argument for condition "
                               "\"%s\"\n",
                               value, locale_key, kf->current_group, condition);
              retval = FALSE;
            }
            break;

          default:
            {
              int argc_diff = -registered_autostart_condition[i].additional_args;

              while (argument && argument[0] != '\0') {
                argc_diff++;
                argument = g_utf8_strchr (argument, -1, ' ');
                while (argument && *argument == ' ')
                  argument++;
              }

              if (argc_diff > 0) {
                print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                                 "has %d too many arguments for condition "
                                 "\"%s\"\n",
                                 value, locale_key, kf->current_group,
                                 argc_diff, condition);
                retval = FALSE;
              } else if (argc_diff < 0) {
                print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                                 "has %d too few arguments for condition "
                                 "\"%s\"\n",
                                 value, locale_key, kf->current_group,
                                 -argc_diff, condition);
                retval = FALSE;
              }
            }
            break;
        }

      }

      break;

    }

    /* Now, if we didn't find condition in list of registered
     * AutostartCondition... */
    if (i == G_N_ELEMENTS (registered_autostart_condition)) {
      /* Accept conditions with same name as OnlyShowIn values */

      for (i = 0; i < G_N_ELEMENTS (show_in_registered); i++) {
        if (!strcmp (condition, show_in_registered[i]))
          break;
      }

      if (i == G_N_ELEMENTS (show_in_registered)) {
        print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                         "contains an unregistered value \"%s\" for the "
                         "condition; values extending the format should "
                         "start with \"X-\"\n",
                         value, locale_key, kf->current_group, condition);
        retval = FALSE;
      }

      if (argument && argument[0] == '\0')
        print_warning (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                           "has trailing space(s)\n",
                           value, locale_key, kf->current_group);
    }
  }

  g_free (condition);

  return retval;
}

static gboolean
handle_key_for_application (kf_validator *kf,
                            const char   *locale_key,
                            const char   *value)
{
  kf->application_keys = g_list_append (kf->application_keys,
                                        g_strdup (locale_key));
  return TRUE;
}

static gboolean
handle_key_for_link (kf_validator *kf,
                     const char   *locale_key,
                     const char   *value)
{
  kf->link_keys = g_list_append (kf->link_keys,
                                 g_strdup (locale_key));
  return TRUE;
}

static gboolean
handle_key_for_fsdevice (kf_validator *kf,
                         const char   *locale_key,
                         const char   *value)
{
  kf->fsdevice_keys = g_list_append (kf->fsdevice_keys,
                                     g_strdup (locale_key));
  return TRUE;
}

static gboolean
handle_key_for_mimetype (kf_validator *kf,
                         const char   *locale_key,
                         const char   *value)
{
  kf->mimetype_keys = g_list_append (kf->mimetype_keys,
                                     g_strdup (locale_key));
  return TRUE;
}

/* + Key names must contain only the characters A-Za-z0-9-.
 *   Checked (through key_is_valid()).
 * + LOCALE must be of the form lang_COUNTRY.ENCODING@MODIFIER, where _COUNTRY,
 *   .ENCODING, and @MODIFIER  may be omitted.
 *   Checked.
 */
static gboolean
key_extract_locale (const char  *key,
                    char       **real_key,
                    char       **locale)
{
  const char *start_locale;
  char        c;
  int         len;
  int         i;

  if (real_key)
    *real_key = NULL;
  if (locale)
    *locale = NULL;

  start_locale = g_strrstr (key, "[");

  if (start_locale)
    len = start_locale - key;
  else
    len = strlen (key);

  if (!key_is_valid(key, len))
    return FALSE;

  if (!start_locale) {
    if (real_key)
      *real_key = g_strdup (key);
    if (locale)
      *locale = NULL;

    return TRUE;
  }

  len = strlen (start_locale);
  if (len <= 2 || start_locale[len - 1] != ']')
    return FALSE;

  /* ignore first [ and last ] */
  for (i = 1; i < len - 2; i++) {
    c = start_locale[i];
    if (!g_ascii_isalnum (c) && c != '-' && c != '_' && c != '.' && c != '@')
      return FALSE;
  }

  if (real_key)
    *real_key = g_strndup (key, strlen (key) - len);
  if (locale)
    *locale = g_strndup (start_locale + 1, len - 2);

  return TRUE;
}

/* + All keys extending the format should start with "X-".
 *   Checked.
 */
static gboolean
validate_known_key (kf_validator         *kf,
                    const char           *locale_key,
                    const char           *key,
                    const char           *locale,
                    const char           *value,
                    DesktopKeyDefinition *keys,
                    unsigned int          n_keys)
{
  unsigned int i;
  unsigned int j;

  for (i = 0; i < n_keys; i++) {
    if (strcmp (key, keys[i].name))
      continue;

    if (keys[i].type != DESKTOP_LOCALESTRING_TYPE &&
        keys[i].type != DESKTOP_LOCALESTRING_LIST_TYPE &&
        locale != NULL) {
      if (!strncmp (key, "X-", 2))
        return TRUE;
      print_fatal (kf, "file contains key \"%s\" in group \"%s\", "
                       "but \"%s\" is not defined as a locale string\n",
                       locale_key, kf->current_group, key);
      return FALSE;
    }

    for (j = 0; j < G_N_ELEMENTS (validate_for_type); j++) {
      if (validate_for_type[j].type == keys[i].type)
        break;
    }

    g_assert (j != G_N_ELEMENTS (validate_for_type));

    if (!kf->no_deprecated_warnings && keys[i].deprecated)
      print_warning (kf, "key \"%s\" in group \"%s\" is deprecated\n",
                         locale_key, kf->current_group);

    if (keys[i].kde_reserved && kf->kde_reserved_warnings)
      print_warning (kf, "key \"%s\" in group \"%s\" is a reserved key for "
                         "KDE\n",
                         locale_key, kf->current_group);

    if (!strncmp (key, "X-", 2))
      return TRUE;

    if (!validate_for_type[j].validate (kf, key, locale, value))
      return FALSE;

    if (keys[i].handle_and_validate != NULL) {
      if (!keys[i].handle_and_validate (kf, locale_key, value))
        return FALSE;
    }

    break;
  }

  if (i == n_keys && strncmp (key, "X-", 2)) {
    print_fatal (kf, "file contains key \"%s\" in group \"%s\", but "
                     "keys extending the format should start with "
                     "\"X-\"\n", key, kf->current_group);
    return FALSE;
  }

  return TRUE;
}

static gboolean
validate_desktop_key (kf_validator *kf,
                      const char   *locale_key,
                      const char   *key,
                      const char   *locale,
                      const char   *value)
{
  return validate_known_key (kf, locale_key, key, locale, value,
                             registered_desktop_keys,
                             G_N_ELEMENTS (registered_desktop_keys));
}

static gboolean
validate_action_key (kf_validator *kf,
                     const char   *locale_key,
                     const char   *key,
                     const char   *locale,
                     const char   *value)
{
  return validate_known_key (kf, locale_key, key, locale, value,
                             registered_action_keys,
                             G_N_ELEMENTS (registered_action_keys));
}

/* + Multiple keys in the same group may not have the same name.
 *   Checked.
 */
static gboolean
validate_keys_for_current_group (kf_validator *kf)
{
  gboolean     desktop_group;
  gboolean     action_group;
  gboolean     retval;
  GHashTable  *duplicated_keys_hash;
  char        *key;
  char        *locale;
  GSList      *keys;
  GSList      *sl;
  gpointer     hashvalue;

  retval = TRUE;

  desktop_group = (!strcmp (kf->current_group, GROUP_DESKTOP_ENTRY) ||
                   !strcmp (kf->current_group, GROUP_KDE_DESKTOP_ENTRY));
  action_group = (!strncmp (kf->current_group, GROUP_DESKTOP_ACTION,
                            strlen (GROUP_DESKTOP_ACTION)));

  keys = g_slist_copy (g_hash_table_lookup (kf->groups, kf->current_group));
  /* keys were prepended, so reverse the list (that's why we use a
   * g_slist_copy() */
  keys = g_slist_reverse (keys);

  kf->current_keys = g_hash_table_new_full (g_str_hash, g_str_equal,
                                            NULL, NULL);
  duplicated_keys_hash = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                NULL, NULL);

  /* we need two passes: some checks are looking if another key exists in the
   * group */
  for (sl = keys; sl != NULL; sl = sl->next) {
    kf_keyvalue *keyvalue;

    keyvalue = (kf_keyvalue *) sl->data;
    g_hash_table_insert (kf->current_keys, keyvalue->key, keyvalue);

    /* we could display the error about duplicate keys here, but it's better
     * to display it with the first occurence of this key */
    hashvalue = g_hash_table_lookup (duplicated_keys_hash, keyvalue->key);
    if (!hashvalue)
      g_hash_table_insert (duplicated_keys_hash, keyvalue->key,
                           GINT_TO_POINTER (1));
    else {
      g_hash_table_replace (duplicated_keys_hash, keyvalue->key,
                            GINT_TO_POINTER (GPOINTER_TO_INT (hashvalue) + 1));
    }
  }

  for (sl = keys; sl != NULL; sl = sl->next) {
    kf_keyvalue *keyvalue;
    gboolean     skip_desktop_check;

    keyvalue = (kf_keyvalue *) sl->data;

    skip_desktop_check = FALSE;

    if (!key_extract_locale (keyvalue->key, &key, &locale)) {
        print_fatal (kf, "file contains key \"%s\" in group \"%s\", but "
                         "key names must contain only the characters "
                         "A-Za-z0-9- (they may have a \"[LOCALE]\" postfix)\n",
                         keyvalue->key, kf->current_group);
        retval = FALSE;
        skip_desktop_check = TRUE;

        key = g_strdup (keyvalue->key);
    }

    g_assert (key != NULL);

    hashvalue = g_hash_table_lookup (duplicated_keys_hash, keyvalue->key);
    if (GPOINTER_TO_INT (hashvalue) > 1) {
      g_hash_table_remove (duplicated_keys_hash, keyvalue->key);
      print_fatal (kf, "file contains multiple keys named \"%s\" in "
                       "group \"%s\"\n", keyvalue->key, kf->current_group);
      retval = FALSE;
    }

    if (desktop_group && !skip_desktop_check) {
      if (!validate_desktop_key (kf, keyvalue->key,
                                 key, locale, keyvalue->value))
        retval = FALSE;
    } else if (action_group && !skip_desktop_check) {
      if (!validate_action_key (kf, keyvalue->key,
                                key, locale, keyvalue->value))
        retval = FALSE;
    }

    g_free (key);
    key = NULL;
    g_free (locale);
    locale = NULL;
  }

  g_slist_free (keys);
  g_hash_table_destroy (duplicated_keys_hash);
  g_hash_table_destroy (kf->current_keys);
  kf->current_keys = NULL;
  /* Clear ShowIn flag, so that different groups can each have a OnlyShowIn /
   * NotShowIn key */
  kf->show_in = FALSE;

  return retval;
}

/* + Using [KDE Desktop Entry] instead of [Desktop Entry] as header is
 *   deprecated.
 *   Checked.
 * + Group names may contain all ASCII characters except for [ and ] and
 *   control characters.
 *   Checked.
 * + All groups extending the format should start with "X-".
 *   Checked.
 * + Accept "Desktop Action foobar" group, where foobar is a valid key
 *   name.
 *   Checked.
 * + Accept a group with the same name as the interface listed in
 *   "Implements"
 *   Checked.
 *
 * Note that for "Desktop Action foobar" group, we will check later on (in
 * validate_actions()) that the Actions key contains "foobar".
 */
static gboolean
validate_group_name (kf_validator *kf,
                     const char   *group)
{
  int  i;
  char c;

  for (i = 0; group[i] != '\0'; i++) {
    c = group[i];
    if (g_ascii_iscntrl (c) || c == '[' || c == ']') {
      print_fatal (kf, "file contains group \"%s\", but group names "
                       "may contain all ASCII characters except for [ "
                       "and ] and control characters\n", group);
      return FALSE;
    }
  }

  if (!strncmp (group, "X-", 2))
    return TRUE;

  if (!strcmp (group, GROUP_DESKTOP_ENTRY)) {
    if (kf->main_group && !strcmp (kf->main_group, GROUP_KDE_DESKTOP_ENTRY))
      print_warning (kf, "file contains groups \"%s\" and \"%s\", which play "
                         "the same role\n",
                         GROUP_KDE_DESKTOP_ENTRY, GROUP_DESKTOP_ENTRY);

    kf->main_group = GROUP_DESKTOP_ENTRY;

    return TRUE;
  }

  if (!strcmp (group, GROUP_KDE_DESKTOP_ENTRY)) {
    if (kf->kde_reserved_warnings || !kf->no_deprecated_warnings)
      print_warning (kf, "file contains group \"%s\", which is deprecated "
                         "in favor of \"%s\"\n", group, GROUP_DESKTOP_ENTRY);

    if (kf->main_group && !strcmp (kf->main_group, GROUP_DESKTOP_ENTRY))
      print_warning (kf, "file contains groups \"%s\" and \"%s\", which play "
                         "the same role\n",
                         GROUP_DESKTOP_ENTRY, GROUP_KDE_DESKTOP_ENTRY);

    kf->main_group = GROUP_KDE_DESKTOP_ENTRY;

    return TRUE;
  }

  if (!strncmp (group, GROUP_DESKTOP_ACTION, strlen (GROUP_DESKTOP_ACTION))) {
    if (group[strlen (GROUP_DESKTOP_ACTION) - 1] == '\0') {
      print_fatal (kf, "file contains group \"%s\", which is an action "
                       "group with no action name\n", group);
      return FALSE;
    } else {
      char *action;

      action = g_strdup (group + strlen (GROUP_DESKTOP_ACTION));

      if (!key_is_valid (action, strlen (action))) {
        print_fatal (kf, "file contains group \"%s\", which has an invalid "
                         "action identifier, only alphanumeric characters and "
                         "'-' are allowed\n", group);
        g_free (action);
        return FALSE;
      }

      g_hash_table_insert (kf->action_groups, action, action);

      return TRUE;
    }
  }

  if (g_hash_table_lookup (kf->interfaces, group))
      return TRUE;

  print_fatal (kf, "file contains group \"%s\", but groups extending "
                   "the format should start with \"X-\"\n", group);
  return FALSE;
}

static gboolean
validate_required_keys (kf_validator         *kf,
                        const char           *group_name,
                        DesktopKeyDefinition *key_definitions,
                        unsigned int          n_keys)
{
  gboolean      retval;
  unsigned int  i;
  GSList       *sl;
  GSList       *keys;
  GHashTable   *hashtable;

  retval = TRUE;

  hashtable = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
  keys = g_hash_table_lookup (kf->groups, group_name);

  for (sl = keys; sl != NULL; sl = sl->next) {
    kf_keyvalue *keyvalue;

    keyvalue = (kf_keyvalue *) sl->data;
    g_hash_table_insert (hashtable, keyvalue->key, keyvalue->key);
  }

  for (i = 0; i < n_keys; i++) {
    if (key_definitions[i].required) {
      if (!g_hash_table_lookup (hashtable,
                                key_definitions[i].name)) {
        print_fatal (kf, "required key \"%s\" in group \"%s\" is not "
                         "present\n",
                         key_definitions[i].name, group_name);
        retval = FALSE;
      }
    }
  }

  g_hash_table_destroy (hashtable);

  return retval;
}

static gboolean
validate_required_desktop_keys (kf_validator *kf)
{
  return validate_required_keys (kf, kf->main_group,
                                 registered_desktop_keys,
                                 G_N_ELEMENTS (registered_desktop_keys));
}

#define PRINT_ERROR_FOREACH_KEY(lower, real)                                 \
static void                                                                  \
print_error_foreach_##lower##_key (const char   *name,                       \
                                   kf_validator *kf)                         \
{                                                                            \
  print_fatal (kf, "key \"%s\" is present in group \"%s\", but the type is " \
                   "\"%s\" while this key is only valid for type \"%s\"\n",  \
                   name, kf->main_group, kf->type_string, real);             \
}

PRINT_ERROR_FOREACH_KEY (application, "Application")
PRINT_ERROR_FOREACH_KEY (link,        "Link")
PRINT_ERROR_FOREACH_KEY (fsdevice,    "FSDevice")
PRINT_ERROR_FOREACH_KEY (mimetype,    "MimeType")

static gboolean
validate_type_keys (kf_validator *kf)
{
  gboolean retval;

  retval = TRUE;

  switch (kf->type) {
    case INVALID_TYPE:
      break;
    case APPLICATION_TYPE:
      g_list_foreach (kf->link_keys,
                      (GFunc) print_error_foreach_link_key, kf);
      g_list_foreach (kf->fsdevice_keys,
                      (GFunc) print_error_foreach_fsdevice_key, kf);
      g_list_foreach (kf->mimetype_keys,
                      (GFunc) print_error_foreach_mimetype_key, kf);
      retval = (g_list_length (kf->link_keys) +
                g_list_length (kf->fsdevice_keys) +
                g_list_length (kf->mimetype_keys) == 0);
      break;
    case LINK_TYPE:
      g_list_foreach (kf->application_keys,
                      (GFunc) print_error_foreach_application_key, kf);
      g_list_foreach (kf->fsdevice_keys,
                      (GFunc) print_error_foreach_fsdevice_key, kf);
      g_list_foreach (kf->mimetype_keys,
                      (GFunc) print_error_foreach_mimetype_key, kf);
      retval = (g_list_length (kf->application_keys) +
                g_list_length (kf->fsdevice_keys) +
                g_list_length (kf->mimetype_keys) == 0);
      break;
    case DIRECTORY_TYPE:
    case SERVICE_TYPE:
    case SERVICE_TYPE_TYPE:
      g_list_foreach (kf->application_keys,
                      (GFunc) print_error_foreach_application_key, kf);
      g_list_foreach (kf->link_keys,
                      (GFunc) print_error_foreach_link_key, kf);
      g_list_foreach (kf->fsdevice_keys,
                      (GFunc) print_error_foreach_fsdevice_key, kf);
      g_list_foreach (kf->mimetype_keys,
                      (GFunc) print_error_foreach_mimetype_key, kf);
      retval = (g_list_length (kf->application_keys) +
                g_list_length (kf->link_keys) +
                g_list_length (kf->fsdevice_keys) +
                g_list_length (kf->mimetype_keys) == 0);
      break;
    case FSDEVICE_TYPE:
      g_list_foreach (kf->application_keys,
                      (GFunc) print_error_foreach_application_key, kf);
      g_list_foreach (kf->link_keys,
                      (GFunc) print_error_foreach_link_key, kf);
      g_list_foreach (kf->mimetype_keys,
                      (GFunc) print_error_foreach_mimetype_key, kf);
      retval = (g_list_length (kf->application_keys) +
                g_list_length (kf->link_keys) +
                g_list_length (kf->mimetype_keys) == 0);
      break;
    case MIMETYPE_TYPE:
      g_list_foreach (kf->application_keys,
                      (GFunc) print_error_foreach_application_key, kf);
      g_list_foreach (kf->link_keys,
                      (GFunc) print_error_foreach_link_key, kf);
      g_list_foreach (kf->fsdevice_keys,
                      (GFunc) print_error_foreach_fsdevice_key, kf);
      retval = (g_list_length (kf->application_keys) +
                g_list_length (kf->link_keys) +
                g_list_length (kf->fsdevice_keys) == 0);
      break;
    case LAST_TYPE:
      g_assert_not_reached ();
  }

  return retval;
}

static gboolean
lookup_group_foreach_action (char         *key,
                             char         *value,
                             kf_validator *kf)
{
  if (g_hash_table_lookup (kf->action_groups, key)) {
    gchar *group_name;

    group_name = g_strconcat (GROUP_DESKTOP_ACTION, key, NULL);
    validate_required_keys (kf, group_name,
                            registered_action_keys,
                            G_N_ELEMENTS (registered_action_keys));
    g_free (group_name);

    g_hash_table_remove (kf->action_groups, key);
    return TRUE;
  }

  return FALSE;
}

static void
print_error_foreach_action (char         *key,
                            char         *value,
                            kf_validator *kf)
{
  print_fatal (kf, "action \"%s\" is defined, but there is no matching "
                   "\"%s%s\" group\n", key, GROUP_DESKTOP_ACTION, key);
}

static void
print_error_foreach_group (char         *key,
                           char         *value,
                           kf_validator *kf)
{
  print_fatal (kf, "action group \"%s%s\" exists, but there is no matching "
                   "action \"%s\"\n", GROUP_DESKTOP_ACTION, key, key);
}

static gboolean
validate_actions (kf_validator *kf)
{
  g_hash_table_foreach_remove (kf->action_values,
                               (GHRFunc) lookup_group_foreach_action, kf);

  g_hash_table_foreach (kf->action_values,
                        (GHFunc) print_error_foreach_action, kf);

  g_hash_table_foreach (kf->action_groups,
                        (GHFunc) print_error_foreach_group, kf);

  return (g_hash_table_size (kf->action_values) +
          g_hash_table_size (kf->action_groups) == 0);
}

/* + These desktop entry files should have the extension .desktop.
 *   Checked.
 * + Desktop entries which describe how a directory is to be
 *   formatted/displayed should be simply called .directory.
 *   Checked.
 * + Using .kdelnk instead of .desktop as the file extension is deprecated.
 *   Checked.
 * FIXME: we're not doing what the spec says wrt Directory.
 */
static gboolean
validate_filename (kf_validator *kf)
{
  if (kf->type == DIRECTORY_TYPE) {
    if (g_str_has_suffix (kf->filename, ".directory"))
      return TRUE;
    else {
      print_fatal (kf, "file is of type \"Directory\", but filename does not "
                       "have a .directory extension\n");
      return FALSE;
    }
  }

  if (g_str_has_suffix (kf->filename, ".desktop"))
    return TRUE;

  if (g_str_has_suffix (kf->filename, ".kdelnk")) {
    if (kf->kde_reserved_warnings || !kf->no_deprecated_warnings)
      print_warning (kf, "filename has a .kdelnk extension, which is "
                         "deprecated in favor of .desktop\n");
    return TRUE;
  }

  print_fatal (kf, "filename does not have a .desktop extension\n");
  return FALSE;
}

/* + Lines beginning with a # and blank lines are considered comments.
 *   Checked.
 */
static gboolean
validate_line_is_comment (kf_validator *kf,
                          const char   *line)
{
  return (*line == '#' || *line == '\0');
}

/* + A group header with name groupname is a line in the format: [groupname]
 *   Checked.
 * + Group names may contain all ASCII characters except for [ and ] and
 *   control characters.
 *   This is done in validate_group_name().
 */
static gboolean
validate_line_looks_like_group (kf_validator  *kf,
                                const char    *line,
                                char         **group)
{
  char     *chomped;
  gboolean  result;

  chomped = g_strdup (line);
  g_strchomp (chomped);

  result = (*chomped == '[' && chomped[strlen (chomped) - 1] == ']');

  if (result && strcmp (chomped, line))
    print_fatal (kf, "line \"%s\" ends with a space, but looks like a group. "
                     "The validation will continue, with the trailing spaces "
                     "ignored.\n", line);

  if (group && result)
    *group = g_strndup (chomped + 1, strlen (chomped) - 2);

  g_free (chomped);

  return result;
}

/* + Space before and after the equals sign should be ignored; the = sign is
 *   the actual delimiter.
 *   Checked.
 */
static gboolean
validate_line_looks_like_entry (kf_validator  *kf,
                                const char    *line,
                                char         **key,
                                char         **value)
{
  char *p;

  p = g_utf8_strchr (line, -1, '=');

  if (!p)
    return FALSE;

  /* key must be non-empty */
  if (*p == line[0])
    return FALSE;

  if (key) {
    *key = g_strndup (line, p - line);
    g_strchomp (*key);
  }
  if (value) {
    *value = g_strdup (p + 1);
    g_strchug (*value);
  }

  return TRUE;
}

/* + Only comments are accepted before the first group.
 *   Checked.
 * + The first group should be "Desktop Entry".
 *   Checked.
 * + Multiple groups may not have the same name.
 *   Checked.
 */
static void
validate_parse_line (kf_validator *kf)
{
  char *line;
  int   len;
  char *group;
  char *key;
  char *value;

  line = kf->parse_buffer->str;
  len  = kf->parse_buffer->len;

  if (!kf->utf8_warning && !g_utf8_validate (line, len, NULL)) {
    print_warning (kf, "file contains lines that are not UTF-8 encoded. There "
                       "is no guarantee the validator will correctly work.\n");
    kf->utf8_warning = TRUE;
  }

  if (g_ascii_isspace (*line)) {
    print_fatal (kf, "line \"%s\" starts with a space. Comment, group and "
                     "key-value lines should not start with a space. The "
                     "validation will continue, with the leading spaces "
                     "ignored.\n", line);
    while (g_ascii_isspace (*line))
      line++;
  }

  if (validate_line_is_comment (kf, line))
    return;

  group = NULL;
  if (validate_line_looks_like_group (kf, line, &group)) {
    if (!kf->current_group &&
        (strcmp (group, GROUP_DESKTOP_ENTRY) &&
         strcmp (group, GROUP_KDE_DESKTOP_ENTRY)))
      print_fatal (kf, "first group is not \"" GROUP_DESKTOP_ENTRY "\"\n");

    if (kf->current_group && strcmp (kf->current_group, group))
      validate_keys_for_current_group (kf);

    if (g_hash_table_lookup_extended (kf->groups, group, NULL, NULL)) {
      print_fatal (kf, "file contains multiple groups named \"%s\", but "
                       "multiple groups may not have the same name\n", group);
    } else {
      validate_group_name (kf, group);
      g_hash_table_insert (kf->groups, g_strdup (group), NULL);
    }

    if (kf->current_group)
      g_free (kf->current_group);
    kf->current_group = group;

    return;
  }

  key = NULL;
  value = NULL;
  if (validate_line_looks_like_entry (kf, line, &key, &value)) {
    if (kf->current_group) {
      GSList      *keys;
      kf_keyvalue *keyvalue;

      keyvalue = g_slice_new (kf_keyvalue);
      keyvalue->key = key;
      keyvalue->value = value;

      keys = g_hash_table_lookup (kf->groups, kf->current_group);
      keys = g_slist_prepend (keys, keyvalue);
      g_hash_table_replace (kf->groups, g_strdup (kf->current_group), keys);
    } else {
      if (key)
        g_free (key);
      if (value)
        g_free (value);

      print_fatal (kf, "file contains entry \"%s\" before the first group, "
                       "but only comments are accepted before the first "
                       "group\n", line);
    }

    return;
  }

  print_fatal (kf, "file contains line \"%s\", which is not a comment, "
                   "a group or an entry\n", line);
}

/* + Desktop entry files are encoded as lines of 8-bit characters separated by
 *   LF characters.
 *   Checked.
 */
static void
validate_parse_data (kf_validator *kf,
                     char         *data,
                     int           length)
{
  int i;

  for (i = 0; i < length; i++) {
    if (data[i] == '\n') {
      if (i > 0 && data[i - 1] == '\r') {
        g_string_erase (kf->parse_buffer, kf->parse_buffer->len - 1, 1);

        if (!kf->cr_error) {
          print_fatal (kf, "file contains at least one line ending with a "
                           "carriage return before the line feed, while lines "
                           "should only be separated by a line feed "
                           "character. First such line is: \"%s\"\n",
                           kf->parse_buffer->str);
          kf->cr_error = TRUE;
        }
      }

      if (kf->parse_buffer->len > 0) {
        validate_parse_line (kf);
        g_string_erase (kf->parse_buffer, 0, -1);
      }

    } else if (data[i] == '\r') {
        if (!kf->cr_error) {
          print_fatal (kf, "file contains at least one line ending with a "
                           "carriage return, while lines should only be "
                           "separated by a line feed character. First such "
                           "line is: \"%s\"\n", kf->parse_buffer->str);
          kf->cr_error = TRUE;
        }

        data[i] = '\n';
        i--;
    } else
      g_string_append_c (kf->parse_buffer, data[i]);
  }
}

static void
validate_flush_parse_buffer (kf_validator *kf)
{
  if (kf->parse_buffer->len > 0) {
      validate_parse_line (kf);
      g_string_erase (kf->parse_buffer, 0, -1);
  }

  if (kf->current_group)
    validate_keys_for_current_group (kf);
}

#define VALIDATE_READ_SIZE 4096
static gboolean
validate_parse_from_fd (kf_validator *kf,
                        int           fd)
{
  int         bytes_read;
  struct stat stat_buf;
  char        read_buf[VALIDATE_READ_SIZE];

  if (fstat (fd, &stat_buf) < 0) {
    print_fatal (kf, "while reading the file: %s\n", g_strerror (errno));
    return FALSE;
  }

  if (!S_ISREG (stat_buf.st_mode)) {
    print_fatal (kf, "file is not a regular file\n");
    return FALSE;
  }

  if (stat_buf.st_size == 0) {
    print_fatal (kf, "file is empty\n");
    return FALSE;
  }

  bytes_read = 0;
  while (1) {
    bytes_read = read (fd, read_buf, VALIDATE_READ_SIZE);

    if (bytes_read == 0)  /* End of File */
      break;

    if (bytes_read < 0) {
      if (errno == EINTR || errno == EAGAIN)
        continue;

      /* let's validate what we already have */
      validate_flush_parse_buffer (kf);

      print_fatal (kf, "while reading the file: %s\n", g_strerror (errno));
      return FALSE;
    }

    validate_parse_data (kf, read_buf, bytes_read);
  }

  validate_flush_parse_buffer (kf);

  return TRUE;
}

static gboolean
validate_load_and_parse (kf_validator *kf)
{
  int      fd;
  gboolean ret;

  fd = g_open (kf->filename, O_RDONLY, 0);

  if (fd < 0) {
    print_fatal (kf, "while reading the file: %s\n", g_strerror (errno));
    return FALSE;
  }

  ret = validate_parse_from_fd (kf, fd);

  close (fd);

  return ret;
}

static gboolean
groups_hashtable_free (gpointer key,
                       gpointer value,
                       gpointer data)
{
  GSList *list;
  GSList *sl;

  list = (GSList *) value;
  for (sl = list; sl != NULL; sl = sl->next) {
    kf_keyvalue *keyvalue;

    keyvalue = (kf_keyvalue *) sl->data;
    g_free (keyvalue->key);
    g_free (keyvalue->value);
    g_slice_free (kf_keyvalue, keyvalue);
  }

  g_slist_free (list);

  return TRUE;
}

gboolean
desktop_file_validate (const char *filename,
                       gboolean    warn_kde,
                       gboolean    no_warn_deprecated,
                       gboolean    no_hints)
{
  kf_validator kf;

  /* just a consistency check */
  g_assert (G_N_ELEMENTS (registered_types) == LAST_TYPE - 1);

  kf.filename               = filename;
  kf.parse_buffer           = g_string_new ("");
  kf.utf8_warning           = FALSE;
  kf.cr_error               = FALSE;
  kf.current_group          = NULL;
  kf.groups                 = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                     g_free, NULL);
  kf.current_keys           = NULL;
  kf.kde_reserved_warnings  = warn_kde;
  kf.no_deprecated_warnings = no_warn_deprecated;
  kf.no_hints               = no_hints;

  kf.main_group       = NULL;
  kf.type             = INVALID_TYPE;
  kf.type_string      = NULL;
  kf.show_in          = FALSE;
  kf.application_keys = NULL;
  kf.link_keys        = NULL;
  kf.fsdevice_keys    = NULL;
  kf.mimetype_keys    = NULL;
  kf.interfaces       = g_hash_table_new_full (g_str_hash, g_str_equal,
                                               NULL, g_free);
  kf.action_values    = g_hash_table_new_full (g_str_hash, g_str_equal,
                                               NULL, g_free);
  kf.action_groups    = g_hash_table_new_full (g_str_hash, g_str_equal,
                                               NULL, g_free);
  kf.fatal_error      = FALSE;
#if GLIB_CHECK_VERSION(2, 50, 0)
  kf.use_colors       = g_log_writer_supports_color (fileno (stdout));
#else
  kf.use_colors       = FALSE;
#endif

  validate_load_and_parse (&kf);
  //FIXME: this does not work well if there are both a Desktop Entry and a KDE
  //Desktop Entry groups since only the last one will be validated for this.
  if (kf.main_group) {
    validate_required_desktop_keys (&kf);
    validate_type_keys (&kf);
  }
  validate_actions (&kf);
  validate_filename (&kf);

  g_list_foreach (kf.application_keys, (GFunc) g_free, NULL);
  g_list_free (kf.application_keys);
  g_list_foreach (kf.link_keys, (GFunc) g_free, NULL);
  g_list_free (kf.link_keys);
  g_list_foreach (kf.fsdevice_keys, (GFunc) g_free, NULL);
  g_list_free (kf.fsdevice_keys);
  g_list_foreach (kf.mimetype_keys, (GFunc) g_free, NULL);
  g_list_free (kf.mimetype_keys);

  g_hash_table_destroy (kf.interfaces);
  g_hash_table_destroy (kf.action_values);
  g_hash_table_destroy (kf.action_groups);

  g_assert (kf.current_keys == NULL);
  /* we can't add an automatic destroy handler for the value because we replace
   * it when adding keys, and this means we'd have to copy the value each time
   * we replace it */
  g_hash_table_foreach_remove (kf.groups, groups_hashtable_free, NULL);
  g_hash_table_destroy (kf.groups);
  g_free (kf.current_group);
  g_string_free (kf.parse_buffer, TRUE);

  return (!kf.fatal_error);
}

/* return FALSE if we were unable to fix the file */
gboolean
desktop_file_fixup (GKeyFile   *keyfile,
                    const char *filename)
{
  if (g_key_file_has_group (keyfile, GROUP_KDE_DESKTOP_ENTRY)) {
    g_printerr ("%s: warning: renaming deprecated \"%s\" group to \"%s\"\n",
                filename, GROUP_KDE_DESKTOP_ENTRY, GROUP_DESKTOP_ENTRY);
    dfu_key_file_rename_group (keyfile,
                               GROUP_KDE_DESKTOP_ENTRY, GROUP_DESKTOP_ENTRY);
  }

  return TRUE;
}
