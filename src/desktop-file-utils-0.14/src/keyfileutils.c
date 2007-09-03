/* keyfileutils.c: useful functions for GKeyFile
 * 
 * Copyright (C) 2007 Vincent Untz <vuntz@gnome.org>
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

#include <string.h>

#include "keyfileutils.h"

gboolean
dfu_key_file_rename_group (GKeyFile   *keyfile,
                           const char *oldgroup,
                           const char *newgroup)
{
  char         **keys;
  char          *value;
  unsigned int   i;

  g_return_val_if_fail (keyfile != NULL, FALSE);

  if (!g_key_file_has_group (keyfile, oldgroup))
    return TRUE;

  keys = g_key_file_get_keys (keyfile, oldgroup, NULL, NULL);
  for (i = 0; keys[i] != NULL; i++) {
    value = g_key_file_get_value (keyfile, oldgroup, keys[i], NULL);
    g_key_file_set_value (keyfile, newgroup, keys[i], value);
    g_free (value);

    value = g_key_file_get_comment (keyfile, oldgroup, keys[i], NULL);
    if (value) {
      g_key_file_set_comment (keyfile, newgroup, keys[i], value, NULL);
      g_free (value);
    }
  }
  g_strfreev (keys);

  value = g_key_file_get_comment (keyfile, oldgroup, NULL, NULL);
  if (value) {
    g_key_file_set_comment (keyfile, newgroup, NULL, value, NULL);
    g_free (value);
  }

  g_key_file_remove_group (keyfile, oldgroup, NULL);

  return TRUE;
}

gboolean
dfu_key_file_copy_key (GKeyFile   *keyfile,
                       const char *fromgroup,
                       const char *fromkey,
                       const char *togroup,
                       const char *tokey)
{
  char *value;

  g_return_val_if_fail (keyfile != NULL, FALSE);

  if (!g_key_file_has_group (keyfile, fromgroup))
    return FALSE;

  value = g_key_file_get_value (keyfile, fromgroup, fromkey, NULL);
  if (!value)
    return FALSE;

  g_key_file_set_value (keyfile, togroup, tokey, value);

  g_free (value);

  return TRUE;
}

void
dfu_key_file_merge_list (GKeyFile   *keyfile,
                         const char *group,
                         const char *key,
                         const char *to_merge)
{
  char **values;
  char  *value;
  char  *str;
  int    i;

  g_return_if_fail (keyfile != NULL);

  values = g_key_file_get_string_list (keyfile, group, key, NULL, NULL);

  if (values) {
    for (i = 0; values[i] != NULL; i++) {
      if (!strcmp (values[i], to_merge)) {
        g_strfreev (values);
        return;
      }
    }

    g_strfreev (values);
  }

  value = g_key_file_get_value (keyfile, group, key, NULL);

  if (value)
    str = g_strconcat (value, to_merge, ";", NULL);
  else
    str = g_strconcat (to_merge, ";", NULL);

  g_key_file_set_value (keyfile, group, key, str);

  g_free (value);
  g_free (str);
}

void
dfu_key_file_remove_list (GKeyFile   *keyfile,
                          const char *group,
                          const char *key,
                          const char *to_remove)
{
  char     **values;
  GString   *value;
  gboolean   found;
  int        i;

  g_return_if_fail (keyfile != NULL);

  found = FALSE;

  value  = g_string_new ("");
  values = g_key_file_get_string_list (keyfile, group, key, NULL, NULL);

  if (values) {
    for (i = 0; values[i] != NULL; i++) {
      if (!strcmp (values[i], to_remove))
        found = TRUE;
      else
        g_string_append_printf (value, "%s;", values[i]);
    }

    g_strfreev (values);
  }

  if (!found) {
    g_string_free (value, TRUE);
    return;
  }

  if (!value->str || value->str[0] == '\0')
    g_key_file_remove_key (keyfile, group, key, NULL);
  else
    g_key_file_set_value (keyfile, group, key, value->str);

  g_string_free (value, TRUE);
}

//FIXME: kill this when bug #309224 is fixed
gboolean
dfu_key_file_to_file (GKeyFile     *keyfile,
                      const char   *file,
                      GError      **error)
{
  char    *filename;
  GError  *write_error;
  char    *data;
  gsize    length;
  gboolean res;

  g_return_val_if_fail (keyfile != NULL, FALSE);
  g_return_val_if_fail (file != NULL, FALSE);

  write_error = NULL;
  data = g_key_file_to_data (keyfile, &length, &write_error);
  if (write_error) {
    g_propagate_error (error, write_error);
    return FALSE;
  }

  if (!g_path_is_absolute (file))
    filename = g_filename_from_uri (file, NULL, &write_error);
  else
    filename = g_filename_from_utf8 (file, -1, NULL, NULL, &write_error);

  if (write_error) {
    g_propagate_error (error, write_error);
    g_free (data);
    return FALSE;
  }

  res = g_file_set_contents (filename, data, length, &write_error);
  g_free (filename);

  if (write_error) {
    g_propagate_error (error, write_error);
    g_free (data);
    return FALSE;
  }

  g_free (data);
  return res;
}
