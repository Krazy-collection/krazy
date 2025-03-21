/* validator.c: validate a desktop entry file
 * vim: set ts=2 sw=2 et: */

/*
 * Copyright (C) 2007, 2008 Vincent Untz <vuntz@gnome.org>
 *
 * A really small portion of this code comes from the old validator.c.
 * The old validator.c was Copyright (C) 2002, 2004  Red Hat, Inc.
 * It was written by:
 *  Mark McLoughlin <mark@skynet.ie>
 *  Havoc Pennington <hp@pobox.com>
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

#include <config.h>
#include <locale.h>

#ifdef HAVE_PLEDGE
#include <unistd.h>
#endif

#include "validate.h"

static gboolean   warn_kde = FALSE;
static gboolean   no_hints = FALSE;
static gboolean   no_warn_deprecated = FALSE;
static gboolean   print_version = FALSE;
static char     **filename = NULL;

static GOptionEntry option_entries[] = {
  { "no-hints", 0, 0, G_OPTION_ARG_NONE, &no_hints, "Do not output hints to improve desktop file", NULL },
  { "no-warn-deprecated", 0, 0, G_OPTION_ARG_NONE, &no_warn_deprecated, "Do not warn about usage of deprecated items", NULL },
  { "warn-kde", 0, 0, G_OPTION_ARG_NONE, &warn_kde, "Warn if KDE extensions to the specification are used", NULL },
  { "version", 0, 0, G_OPTION_ARG_NONE, &print_version, "Show the program version", NULL },
  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &filename, NULL, "<desktop-file>..." },
  { NULL }
};

int
main (int argc, char *argv[])
{
  GOptionContext *context;
  GError         *error;
  int i;
  gboolean all_valid;

#ifdef HAVE_PLEDGE
  if (pledge ("stdio rpath", NULL) == -1) {
    g_printerr ("pledge\n");
    return 1;
  }
#endif

  setlocale (LC_ALL, "");

  context = g_option_context_new (NULL);
  g_option_context_set_summary (context, "Validate desktop entry files "
                                         "according to the Desktop Entry "
                                         "specification "CURRENT_SPEC_VERSION
                                         ".\n"
                                         "For information about this "
                                         "specification, see:\n\t"
                                         "http://freedesktop.org/wiki/Specifications/desktop-entry-spec");
  g_option_context_add_main_entries (context, option_entries, NULL);

  error = NULL;
  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_printerr ("Error while parsing arguments: %s\n", error->message);
    g_error_free (error);
    return 1;
  }

  g_option_context_free (context);

  if (print_version) {
    g_print("desktop-file-validate %s\n", VERSION);
    return 0;
  }

  if (filename == NULL || filename[0] == NULL) {
    g_printerr ("See \"%s --help\" for correct usage.\n", g_get_prgname ());
    return 1;
  }

  all_valid = TRUE;
  for (i = 0; filename[i]; i++) {
    if (!g_file_test (filename[i], G_FILE_TEST_IS_REGULAR)) {
      g_printerr ("%s: file does not exist\n", filename[i]);
      all_valid = FALSE;
    } else if (!desktop_file_validate (filename[i], warn_kde, no_warn_deprecated, no_hints))
      all_valid = FALSE;
  }

  if (!all_valid)
    return 1;

  return 0;
}
