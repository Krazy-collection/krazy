/* vim: set ts=2 sw=2 et: */

/*
 * Copyright (C) 2002, 2004  Red Hat, Inc.
 * Copyright (C) 2006-2008  Vincent Untz
 *
 * Program written by Havoc Pennington <hp@pobox.com>
 *                    Ray Strode <rstrode@redhat.com>
 *                    Vincent Untz <vuntz@gnome.org>
 *
 * update-desktop-database is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * update-desktop-database is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with update-desktop-database; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 59 Temple Place - Suite
 * 330, Boston, MA 02111-1307, USA.
 */

#include <config.h>

#include <glib.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <locale.h>

#include "keyfileutils.h"
#include "validate.h"

static gboolean edit_mode = FALSE;
static const char** args = NULL;
static gboolean delete_original = FALSE;
static gboolean rebuild_mime_info_cache = FALSE;
static gboolean print_version = FALSE;
static char *vendor_name = NULL;
static char *target_dir = NULL;
static GSList *edit_actions = NULL;
static mode_t permissions = 0644;

typedef enum
{
  DFU_SET_KEY_BUILDING, /* temporary type to create an action in multiple steps */
  DFU_SET_KEY,
  DFU_REMOVE_KEY,
  DFU_ADD_TO_LIST,
  DFU_REMOVE_FROM_LIST,
  DFU_COPY_KEY
} DfuEditActionType;

typedef struct
{
  DfuEditActionType  type;
  char              *key;
  char              *action_value;
} DfuEditAction;

static DfuEditAction *
dfu_edit_action_new (DfuEditActionType  type,
                     const char        *key,
                     const char        *action_value)
{
  DfuEditAction *action;

  action = g_slice_new0 (DfuEditAction);
  action->type = type;
  action->key = g_strdup (key);
  action->action_value = g_strdup (action_value);

  return action;
}

static void
dfu_edit_action_free (DfuEditAction *action)
{
  g_assert (action != NULL);

  g_free (action->key);
  g_free (action->action_value);

  g_slice_free (DfuEditAction, action);
}

static gboolean
files_are_the_same (const char *first,
                    const char *second)
{
  /* This check gets confused by hard links.
   * but it's too annoying to check if two
   * paths are the same (though I'm sure there's a
   * "path canonicalizer" I should be using...)
   */

  struct stat first_sb;
  struct stat second_sb;

  if (stat (first, &first_sb) < 0)
    {
      g_printerr (_("Could not stat \"%s\": %s\n"), first, g_strerror (errno));
      return TRUE;
    }

  if (stat (second, &second_sb) < 0)
    {
      g_printerr (_("Could not stat \"%s\": %s\n"), first, g_strerror (errno));
      return TRUE;
    }

  return ((first_sb.st_dev == second_sb.st_dev) &&
          (first_sb.st_ino == second_sb.st_ino) &&
          /* Broken paranoia in case an OS doesn't use inodes or something */
          (first_sb.st_size == second_sb.st_size) &&
          (first_sb.st_mtime == second_sb.st_mtime));
}

static gboolean
rebuild_cache (const char  *dir,
               GError     **err)
{
  GError *spawn_error;
  char *argv[4] = { "update-desktop-database", "-q", (char *) dir, NULL };
  int exit_status;
  gboolean retval;

  spawn_error = NULL;

  retval = g_spawn_sync (NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
                         NULL, NULL, &exit_status, &spawn_error);

  if (spawn_error != NULL)
    {
      g_propagate_error (err, spawn_error);
      return FALSE;
    }

  return exit_status == 0 && retval;
}

static void
process_one_file (const char *filename,
                  GError    **err)
{
  char *new_filename;
  GKeyFile *kf = NULL;
  GError *rebuild_error;
  GSList *tmp;

  kf = g_key_file_new ();
  if (!g_key_file_load_from_file (kf, filename,
                                  G_KEY_FILE_KEEP_COMMENTS|
                                  G_KEY_FILE_KEEP_TRANSLATIONS,
                                  err)) {
    g_key_file_free (kf);
    return;
  }

  if (!desktop_file_fixup (kf, filename)) {
    g_key_file_free (kf);
    g_set_error (err, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_PARSE,
                 _("Failed to fix the content of the desktop file"));
    return;
  }

  /* Mark file as having been processed by us, so automated
   * tools can check that desktop files went through our
   * munging
   */
  g_key_file_set_string (kf, GROUP_DESKTOP_ENTRY,
                         "X-Desktop-File-Install-Version", VERSION);

  tmp = edit_actions;
  while (tmp != NULL)
    {
      DfuEditAction *action = tmp->data;

      switch (action->type)
        {
          case DFU_SET_KEY:
            g_key_file_set_string (kf, GROUP_DESKTOP_ENTRY,
                                   action->key, action->action_value);
            dfu_key_file_drop_locale_strings (kf, GROUP_DESKTOP_ENTRY,
                                              action->key);
            break;
          case DFU_REMOVE_KEY:
            g_key_file_remove_key (kf, GROUP_DESKTOP_ENTRY,
                                   action->key, NULL);
            dfu_key_file_drop_locale_strings (kf, GROUP_DESKTOP_ENTRY,
                                              action->key);
            break;
          case DFU_ADD_TO_LIST:
            dfu_key_file_merge_list (kf, GROUP_DESKTOP_ENTRY,
                                     action->key, action->action_value);
            break;
          case DFU_REMOVE_FROM_LIST:
            dfu_key_file_remove_list (kf, GROUP_DESKTOP_ENTRY,
                                      action->key, action->action_value);
            break;
          case DFU_COPY_KEY:
            dfu_key_file_copy_key (kf, GROUP_DESKTOP_ENTRY, action->key,
                                   GROUP_DESKTOP_ENTRY, action->action_value);
            break;
          default:
            g_assert_not_reached ();
        }

      tmp = tmp->next;
    }

  if (edit_mode)
    {
      new_filename = g_strdup (filename);
    }
  else
    {
      char *basename = g_path_get_basename (filename);

      if (vendor_name && !g_str_has_prefix (basename, vendor_name))
        {
          char *new_base;
          new_base = g_strconcat (vendor_name, "-", basename, NULL);
          new_filename = g_build_filename (target_dir, new_base, NULL);
          g_free (new_base);
        }
      else
        {
          new_filename = g_build_filename (target_dir, basename, NULL);
        }

      g_free (basename);
    }

  if (!dfu_key_file_to_path (kf, new_filename, err)) {
    g_key_file_free (kf);
    g_free (new_filename);
    return;
  }

  g_key_file_free (kf);

  /* Load and validate the file we just wrote */
  if (!desktop_file_validate (new_filename, FALSE, TRUE, TRUE))
    {
      g_set_error (err, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_PARSE,
                   _("Failed to validate the created desktop file"));

      if (!files_are_the_same (filename, new_filename))
        g_unlink (new_filename);

      g_free (new_filename);
      return;
    }

  if (!edit_mode)
    {
      if (g_chmod (new_filename, permissions) < 0)
        {
          g_set_error (err, G_FILE_ERROR,
                       g_file_error_from_errno (errno),
                       _("Failed to set permissions %o on \"%s\": %s"),
                       permissions, new_filename, g_strerror (errno));

          if (!files_are_the_same (filename, new_filename))
            g_unlink (new_filename);

          g_free (new_filename);
          return;
        }

      if (delete_original &&
          !files_are_the_same (filename, new_filename))
        {
          if (g_unlink (filename) < 0)
            g_printerr (_("Error removing original file \"%s\": %s\n"),
                        filename, g_strerror (errno));
        }
    }

  if (rebuild_mime_info_cache)
    {
      rebuild_error = NULL;
      rebuild_cache (target_dir, &rebuild_error);

      if (rebuild_error != NULL)
        g_propagate_error (err, rebuild_error);
    }

  g_free (new_filename);
}

static gboolean parse_install_options_callback (const gchar  *option_name,
                                                const gchar  *value,
                                                gpointer      data,
                                                GError      **error);

static gboolean parse_edit_options_callback (const gchar  *option_name,
                                             const gchar  *value,
                                             gpointer      data,
                                             GError      **error);


static const GOptionEntry main_options[] = {
  {
    "rebuild-mime-info-cache",
    '\0',
    '\0',
    G_OPTION_ARG_NONE,
    &rebuild_mime_info_cache,
    N_("Rebuild the MIME types application database after processing desktop files"),
    NULL
  },
  {
    "version",
    '\0',
    '\0',
    G_OPTION_ARG_NONE,
    &print_version,
    N_("Show the program version"),
    NULL
  },
  {
    "edit-mode",
    '\0',
    G_OPTION_FLAG_HIDDEN, /* just for development purpose */
    G_OPTION_ARG_NONE,
    &edit_mode,
    N_("Force edit mode"),
    NULL
  },
  { G_OPTION_REMAINING,
    0,
    0,
    G_OPTION_ARG_FILENAME_ARRAY,
    &args,
    NULL,
    N_("[FILE...]") },
  {
    NULL
  }
};

static const GOptionEntry install_options[] = {
  {
#define OPTION_DIR "dir"
    OPTION_DIR,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_install_options_callback,
    N_("Install desktop files to the DIR directory"),
    N_("DIR")
  },
  {
#define OPTION_MODE "mode"
    OPTION_MODE,
    'm',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_install_options_callback,
    N_("Set the permissions of the destination files to MODE"),
    N_("MODE")
  },
  {
#define OPTION_VENDOR "vendor"
    OPTION_VENDOR,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_install_options_callback,
    N_("Add a vendor prefix to the desktop files, if not already present"),
    N_("VENDOR")
  },
  {
    "delete-original",
    '\0',
    '\0',
    G_OPTION_ARG_NONE,
    &delete_original,
    N_("Delete the source desktop files, leaving only the target files (effectively \"renames\" the desktop files)"),
    NULL
  },
  {
    NULL
  }
};

static const GOptionEntry edit_options[] = {
  {
#define OPTION_SET_KEY "set-key"
    OPTION_SET_KEY,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Set the KEY key to VALUE passed to next --set-value option"),
    N_("KEY")
  },
  {
#define OPTION_SET_VALUE "set-value"
    OPTION_SET_VALUE,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Set the KEY key from previous --set-key option to VALUE"),
    N_("VALUE")
  },
  {
#define OPTION_SET_NAME "set-name"
    OPTION_SET_NAME,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Set the \"Name\" key to NAME"),
    N_("NAME")
  },
  {
#define OPTION_COPY_GENERIC_NAME "copy-generic-name-to-name"
    OPTION_COPY_GENERIC_NAME,
    '\0',
    G_OPTION_FLAG_NO_ARG,
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Copy the value of the \"GenericName\" key to the \"Name\" key"),
    NULL
  },
  {
#define OPTION_SET_GENERIC_NAME "set-generic-name"
    OPTION_SET_GENERIC_NAME,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Set the \"GenericName\" key to GENERIC-NAME"),
    N_("GENERIC-NAME")
  },
  {
#define OPTION_COPY_NAME "copy-name-to-generic-name"
    OPTION_COPY_NAME,
    '\0',
    G_OPTION_FLAG_NO_ARG,
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Copy the value of the \"Name\" key to the \"GenericName\" key"),
    NULL
  },
  {
#define OPTION_SET_COMMENT "set-comment"
    OPTION_SET_COMMENT,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Set the \"Comment\" key to COMMENT"),
    N_("COMMENT")
  },
  {
#define OPTION_SET_ICON "set-icon"
    OPTION_SET_ICON,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Set the \"Icon\" key to ICON"),
    N_("ICON")
  },
  {
#define OPTION_ADD_CATEGORY "add-category"
    OPTION_ADD_CATEGORY,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Add CATEGORY to the list of categories"),
    N_("CATEGORY")
  },
  {
#define OPTION_REMOVE_CATEGORY "remove-category"
    OPTION_REMOVE_CATEGORY,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Remove CATEGORY from the list of categories"),
    N_("CATEGORY")
  },
  {
#define OPTION_ADD_MIME_TYPE "add-mime-type"
    OPTION_ADD_MIME_TYPE,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Add MIME-TYPE to the list of MIME types"),
    N_("MIME-TYPE")
  },
  {
#define OPTION_REMOVE_MIME_TYPE "remove-mime-type"
    OPTION_REMOVE_MIME_TYPE,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Remove MIME-TYPE from the list of MIME types"),
    N_("MIME-TYPE")
  },
  {
#define OPTION_ADD_ONLY_SHOW_IN "add-only-show-in"
    OPTION_ADD_ONLY_SHOW_IN,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Add ENVIRONMENT to the list of desktop environment where the desktop files should be displayed"),
    N_("ENVIRONMENT")
  },
  {
#define OPTION_REMOVE_ONLY_SHOW_IN "remove-only-show-in"
    OPTION_REMOVE_ONLY_SHOW_IN,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Remove ENVIRONMENT from the list of desktop environment where the desktop files should be displayed"),
    N_("ENVIRONMENT")
  },
  {
#define OPTION_ADD_NOT_SHOW_IN "add-not-show-in"
    OPTION_ADD_NOT_SHOW_IN,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Add ENVIRONMENT to the list of desktop environment where the desktop files should not be displayed"),
    N_("ENVIRONMENT")
  },
  {
#define OPTION_REMOVE_NOT_SHOW_IN "remove-not-show-in"
    OPTION_REMOVE_NOT_SHOW_IN,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Remove ENVIRONMENT from the list of desktop environment where the desktop files should not be displayed"),
    N_("ENVIRONMENT")
  },
  {
#define OPTION_REMOVE_KEY "remove-key"
    OPTION_REMOVE_KEY,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_edit_options_callback,
    N_("Remove the KEY key from the desktop files, if present"),
    N_("KEY")
  },
  {
    NULL
  }
};

static gboolean
parse_install_options_callback (const gchar  *option_name,
                                const gchar  *value,
                                gpointer      data,
                                GError      **error)
{
  /* skip "-" or "--" */
  option_name++;
  if (*option_name == '-')
    option_name++;

  if (strcmp (OPTION_DIR, option_name) == 0)
    {
      if (target_dir)
        {
          g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                       _("Can only specify --dir once"));
          return FALSE;
        }

      target_dir = g_strdup (value);
    }

  else if (strcmp (OPTION_MODE, option_name) == 0 ||
           strcmp ("m", option_name) == 0)
    {
      unsigned long ul;
      char *end;

      end = NULL;
      ul = strtoul (value, &end, 8);
      if (*value && end && *end == '\0')
        permissions = ul;
      else
        {
          g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                       _("Could not parse mode string \"%s\""), value);

          return FALSE;
        }
    }

  else if (strcmp (OPTION_VENDOR, option_name) == 0)
    {
      if (vendor_name)
        {
          g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
              _("Can only specify --vendor once"));
          return FALSE;
        }

      vendor_name = g_strdup (value);
    }

  else
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                   _("Unknown option \"%s\""), option_name);

      return FALSE;
    }

  return TRUE;
}

static gboolean
parse_edit_options_callback (const gchar  *option_name,
                             const gchar  *value,
                             gpointer      data,
                             GError      **error)
{
  /* Note: we prepend actions to the list, so we'll need to reverse it later */

  DfuEditAction  *action;
  char          **list;
  int             i;

  /* skip "-" or "--" */
  option_name++;
  if (*option_name == '-')
    option_name++;

#define PARSE_OPTION_LIST(type, key)                                    \
  do {                                                                  \
    list = g_strsplit (value, ";", 0);                                  \
    for (i = 0; list[i]; i++)                                           \
      {                                                                 \
        if (*list[i] == '\0')                                           \
          continue;                                                     \
                                                                        \
        action = dfu_edit_action_new (type, key, list[i]);              \
        edit_actions = g_slist_prepend (edit_actions, action);          \
      }                                                                 \
  } while (0)

  if (strcmp (OPTION_SET_KEY, option_name) == 0)
    {
      action = dfu_edit_action_new (DFU_SET_KEY_BUILDING, value, NULL);
      edit_actions = g_slist_prepend (edit_actions, action);
    }

  else if (strcmp (OPTION_SET_VALUE, option_name) == 0)
    {
      gboolean handled = FALSE;

      if (edit_actions != NULL)
        {
          action = edit_actions->data;
          if (action->type == DFU_SET_KEY_BUILDING)
            {
              action->type = DFU_SET_KEY;
              action->action_value = g_strdup (value);

              handled = TRUE;
            }
        }

      if (!handled)
        {
          g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                       _("Option \"--%s\" used without a prior \"--%s\" option"),
                       OPTION_SET_VALUE, OPTION_SET_KEY);

          return FALSE;
        }
    }

  else if (strcmp (OPTION_SET_NAME, option_name) == 0)
    {
      action = dfu_edit_action_new (DFU_SET_KEY, "Name", value);
      edit_actions = g_slist_prepend (edit_actions, action);
    }

  else if (strcmp (OPTION_COPY_GENERIC_NAME, option_name) == 0)
    {
      action = dfu_edit_action_new (DFU_COPY_KEY, "GenericName", "Name");
      edit_actions = g_slist_prepend (edit_actions, action);
    }

  else if (strcmp (OPTION_SET_GENERIC_NAME, option_name) == 0)
    {
      action = dfu_edit_action_new (DFU_SET_KEY, "GenericName", value);
      edit_actions = g_slist_prepend (edit_actions, action);
    }

  else if (strcmp (OPTION_COPY_NAME, option_name) == 0)
    {
      action = dfu_edit_action_new (DFU_COPY_KEY, "Name", "GenericName");
      edit_actions = g_slist_prepend (edit_actions, action);
    }

  else if (strcmp (OPTION_SET_COMMENT, option_name) == 0)
    {
      action = dfu_edit_action_new (DFU_SET_KEY, "Comment", value);
      edit_actions = g_slist_prepend (edit_actions, action);
    }

  else if (strcmp (OPTION_SET_ICON, option_name) == 0)
    {
      action = dfu_edit_action_new (DFU_SET_KEY, "Icon", value);
      edit_actions = g_slist_prepend (edit_actions, action);
    }

  else if (strcmp (OPTION_ADD_CATEGORY, option_name) == 0)
    {
      PARSE_OPTION_LIST (DFU_ADD_TO_LIST, "Categories");
    }

  else if (strcmp (OPTION_REMOVE_CATEGORY, option_name) == 0)
    {
      PARSE_OPTION_LIST (DFU_REMOVE_FROM_LIST, "Categories");
    }

  else if (strcmp (OPTION_ADD_MIME_TYPE, option_name) == 0)
    {
      PARSE_OPTION_LIST (DFU_ADD_TO_LIST, "MimeType");
    }

  else if (strcmp (OPTION_REMOVE_MIME_TYPE, option_name) == 0)
    {
      PARSE_OPTION_LIST (DFU_REMOVE_FROM_LIST, "MimeType");
    }

  else if (strcmp (OPTION_ADD_ONLY_SHOW_IN, option_name) == 0)
    {
      PARSE_OPTION_LIST (DFU_ADD_TO_LIST, "OnlyShowIn");
    }

  else if (strcmp (OPTION_REMOVE_ONLY_SHOW_IN, option_name) == 0)
    {
      PARSE_OPTION_LIST (DFU_REMOVE_FROM_LIST, "OnlyShowIn");
    }

  else if (strcmp (OPTION_ADD_NOT_SHOW_IN, option_name) == 0)
    {
      PARSE_OPTION_LIST (DFU_ADD_TO_LIST, "NotShowIn");
    }

  else if (strcmp (OPTION_REMOVE_NOT_SHOW_IN, option_name) == 0)
    {
      PARSE_OPTION_LIST (DFU_REMOVE_FROM_LIST, "NotShowIn");
    }

  else if (strcmp (OPTION_REMOVE_KEY, option_name) == 0)
    {
      action = dfu_edit_action_new (DFU_REMOVE_KEY, value, NULL);
      edit_actions = g_slist_prepend (edit_actions, action);
    }

  else
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                   _("Unknown option \"%s\""), option_name);

      return FALSE;
    }

  return TRUE;
}

static gboolean
check_no_building_in_edit_actions (GError **error)
{
  GSList *tmp = edit_actions;

  while (tmp != NULL)
    {
      /* If we have an action that is BUILDING, then it means that we had a
       * --set-key not followed by a --set-value. This can happen when:
       *   + the very last argument was --set-key
       *   + --set-key was followed by another editing option
       * In both cases, that's bad as what we want is a --set-value following
       * each --set-key.
       */
      DfuEditAction *action = tmp->data;

      if (action->type == DFU_SET_KEY_BUILDING)
        {
          g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                       _("Option \"--%s\" used without a following \"--%s\" option"),
                       OPTION_SET_KEY, OPTION_SET_VALUE);

          return FALSE;
        }

      tmp = tmp->next;
    }

  return TRUE;
}

static gboolean
post_parse_edit_options_callback (GOptionContext  *context,
                                  GOptionGroup    *group,
                                  gpointer         data,
                                  GError         **error)
{
  if (!check_no_building_in_edit_actions (error))
    return FALSE;

  /* Reverse list we created by prepending elements */
  edit_actions = g_slist_reverse (edit_actions);

  return TRUE;
}

int
main (int argc, char **argv)
{
  GOptionContext *context;
  GOptionGroup *group;
  GError* err = NULL;
  int i;
  int args_len;
  mode_t dir_permissions;
  char *basename;

#ifdef HAVE_PLEDGE
  if (pledge ("stdio rpath wpath cpath fattr", NULL) == -1) {
    g_printerr ("pledge\n");
    return 1;
  }
#endif

  setlocale (LC_ALL, "");

  basename = g_path_get_basename (argv[0]);
  if (g_strcmp0 (basename, "desktop-file-edit") == 0)
    edit_mode = TRUE;
  g_free (basename);

  context = g_option_context_new ("");
  g_option_context_set_summary (context, edit_mode ? _("Edit a desktop file.") : _("Install desktop files."));
  g_option_context_add_main_entries (context, main_options, NULL);

  if (!edit_mode)
    {
      group = g_option_group_new ("install", _("Installation options for desktop file"), _("Show desktop file installation options"), NULL, NULL);
      g_option_group_add_entries (group, install_options);
      g_option_context_add_group (context, group);
    }
#ifdef HAVE_PLEDGE
  else
    {
      /* In edit mode we can drop the fattr pledge. */
      if (pledge ("stdio rpath wpath cpath", NULL) == -1) {
        g_printerr ("pledge in edit_mode\n");
        return 1;
      }
    }
#endif

  group = g_option_group_new ("edit", _("Edition options for desktop file"), _("Show desktop file edition options"), NULL, NULL);
  g_option_group_add_entries (group, edit_options);
  g_option_group_set_parse_hooks (group, NULL, post_parse_edit_options_callback);
  g_option_context_add_group (context, group);

  err = NULL;
  g_option_context_parse (context, &argc, &argv, &err);

  if (err != NULL) {
    g_printerr ("%s\n", err->message);
    g_printerr (_("Run '%s --help' to see a full list of available command line options.\n"), argv[0]);
    g_error_free (err);
    return 1;
  }

  if (print_version) {
    g_print("desktop-file-install %s\n", VERSION);
    return 0;
  }

  if (!edit_mode)
    {
      if (vendor_name == NULL && g_getenv ("DESKTOP_FILE_VENDOR"))
        vendor_name = g_strdup (g_getenv ("DESKTOP_FILE_VENDOR"));

      if (target_dir == NULL && g_getenv ("DESKTOP_FILE_INSTALL_DIR"))
        target_dir = g_strdup (g_getenv ("DESKTOP_FILE_INSTALL_DIR"));

      if (target_dir == NULL)
        {
          if (g_getenv ("RPM_BUILD_ROOT"))
            target_dir = g_build_filename (g_getenv ("RPM_BUILD_ROOT"), DATADIR, "applications", NULL);
          else
            target_dir = g_build_filename (DATADIR, "applications", NULL);
        }

      /* Create the target directory */
      dir_permissions = permissions;

      /* Add search bit when the target file is readable */
      if (permissions & 0400)
        dir_permissions |= 0100;
      if (permissions & 0040)
        dir_permissions |= 0010;
      if (permissions & 0004)
        dir_permissions |= 0001;

      g_mkdir_with_parents (target_dir, dir_permissions);
    }

  args_len = 0;
  for (i = 0; args && args[i]; i++)
    args_len++;

  if (edit_mode)
    {
      if (args_len == 0)
        {
          g_printerr (_("Must specify a desktop file to process.\n"));
          return 1;
        }
      if (args_len > 1)
        {
          g_printerr (_("Only one desktop file can be processed at once.\n"));
          return 1;
        }
    }
  else
    {
      if (args_len == 0)
        {
          g_printerr (_("Must specify one or more desktop files to process.\n"));
          return 1;
        }
    }

  for (i = 0; args && args[i]; i++)
    {
      err = NULL;
      process_one_file (args[i], &err);
      if (err != NULL)
        {
          g_printerr (_("Error on file \"%s\": %s\n"),
                      args[i], err->message);
          g_error_free (err);

          return 1;
        }
    }

#if GLIB_CHECK_VERSION(2,28,0)
  g_slist_free_full (edit_actions, (GDestroyNotify) dfu_edit_action_free);
#else
  g_slist_foreach (edit_actions, (GFunc) dfu_edit_action_free, NULL);
  g_slist_free (edit_actions);
#endif

  g_option_context_free (context);

  return 0;
}
