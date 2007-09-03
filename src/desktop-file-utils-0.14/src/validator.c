/* validator.c: validate a desktop entry file
 * 
 * Copyright (C) 2007 Vincent Untz <vuntz@gnome.org>
 *
 * A really small portion of this code comes from the old validator.c.
 * Authors of the old validator.c are:
 *  Mark McLoughlin
 *  Havoc Pennington
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

#include "validate.h"

static gboolean   warn_kde = TRUE;
static gboolean   no_warn_deprecated = FALSE;
static char     **filename = NULL;

static GOptionEntry option_entries[] = {
  { "warn-kde", 0, 0, G_OPTION_ARG_NONE, &warn_kde, "Warn about usage of KDE extensions to the specification", NULL },
  { "no-warn-deprecated", 0, 0, G_OPTION_ARG_NONE, &no_warn_deprecated, "Do not warn about usage of deprecated items", NULL },
  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &filename, NULL, "<desktop-file>" },
  { NULL }
};

int 
main (int argc, char *argv[])
{
  GOptionContext *context;
  GError         *error;

  context = g_option_context_new (NULL);
  g_option_context_set_summary (context, "Validate desktop entry files "
                                         "according to the desktop entry "
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

  /* only accept one desktop file argument */
  if (filename == NULL || filename[0] == NULL || filename[1] != NULL) {
    g_printerr ("See \"%s --help\" for correct usage.\n", g_get_prgname ());
    return 1;
  }

  if (!g_file_test (filename[0], G_FILE_TEST_IS_REGULAR)) {
    g_printerr ("%s: file does not exist\n", filename[0]);
    return 1;
  }

  if (desktop_file_validate (filename[0], warn_kde, no_warn_deprecated))
    return 0;
  else
    return 1;
}
