/* update-desktop-database.c - maintains mimetype<->desktop mapping cache
 * vim: set ts=2 sw=2 et: */

/*
 * Copyright (C) 2004-2006  Red Hat, Inc.
 * Copyright (C) 2006, 2008  Vincent Untz
 *
 * Program written by Ray Strode <rstrode@redhat.com>
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
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <glib.h>
#include <glib/gi18n.h>

#include "keyfileutils.h"
#include "mimeutils.h"

#define NAME "update-desktop-database"
#define CACHE_FILENAME "mimeinfo.cache"
#define TEMP_CACHE_FILENAME_PREFIX ".mimeinfo.cache.XXXXXX"

#define udd_print(...) if (!quiet) g_printerr (__VA_ARGS__)
#define udd_verbose_print(...) if (!quiet && verbose) g_printerr (__VA_ARGS__)

static FILE *open_temp_cache_file (const char  *dir,
                                   char       **filename,
                                   GError     **error);
static void add_mime_type (const char *mime_type, GList *desktop_files, FILE *f);
static void sync_database (const char *dir, GError **error);
static void cache_desktop_file (const char  *desktop_file,
                                const char  *mime_type,
                                GError     **error);
static void process_desktop_file (const char  *desktop_file,
                                  const char  *name,
                                  GError     **error);
static void process_desktop_files (const char *desktop_dir,
                                   const char *prefix,
                                   GError **error);
static void update_database (const char *desktop_dir, GError **error);
static const char ** get_default_search_path (void);
static void print_desktop_dirs (const char **dirs);

static GHashTable *mime_types_map = NULL;
static gboolean verbose = FALSE, quiet = FALSE, print_version = FALSE;

static void
list_free_deep (gpointer key, GList *l, gpointer data)
{
  g_list_foreach (l, (GFunc)g_free, NULL);
  g_list_free (l);
}

static void
cache_desktop_file (const char  *desktop_file,
                    const char  *mime_type,
                    GError     **error)
{
  GList *desktop_files;

  desktop_files = (GList *) g_hash_table_lookup (mime_types_map, mime_type);

  /* do not add twice a desktop file mentioning the mime type more than once
   * (no need to use g_list_find() because we cache all mime types registered
   * by a desktop file before moving to another desktop file) */
  if (desktop_files &&
      strcmp (desktop_file, (const char *) desktop_files->data) == 0)
    return;

  desktop_files = g_list_prepend (desktop_files, g_strdup (desktop_file));
  g_hash_table_insert (mime_types_map, g_strdup (mime_type), desktop_files);
}


static void
process_desktop_file (const char  *desktop_file,
                      const char  *name,
                      GError     **error)
{
  GError *load_error;
  GKeyFile *keyfile;
  char **mime_types;
  int i;

  keyfile = g_key_file_new ();

  load_error = NULL;
  g_key_file_load_from_file (keyfile, desktop_file,
                             G_KEY_FILE_NONE, &load_error);

  if (load_error != NULL)
    {
      g_propagate_error (error, load_error);
      return;
    }

  /* Hidden=true means that the .desktop file should be completely ignored */
  if (g_key_file_get_boolean (keyfile, GROUP_DESKTOP_ENTRY, "Hidden", NULL))
    {
      g_key_file_free (keyfile);
      return;
    }

  mime_types = g_key_file_get_string_list (keyfile,
                                           GROUP_DESKTOP_ENTRY,
                                           "MimeType", NULL, &load_error);

  g_key_file_free (keyfile);

  if (load_error != NULL)
    {
      g_propagate_error (error, load_error);
      return;
    }

  for (i = 0; mime_types[i] != NULL; i++)
    {
      char *mime_type;
      MimeUtilsValidity valid;
      char *valid_error;

      mime_type = g_strchomp (mime_types[i]);
      valid = mu_mime_type_is_valid (mime_types[i], &valid_error);
      switch (valid)
      {
        case MU_VALID:
          break;
        case MU_DISCOURAGED:
          udd_print (_("Warning in file \"%s\": usage of MIME type \"%s\" is "
                       "discouraged (%s)\n"),
                     desktop_file, mime_types[i], valid_error);
          g_free (valid_error);
          break;
        case MU_INVALID:
          udd_print (_("Error in file \"%s\": \"%s\" is an invalid MIME type "
                       "(%s)\n"),
                     desktop_file, mime_types[i], valid_error);
          g_free (valid_error);
          /* not a break: we continue to the next mime type */
          continue;
        default:
          g_assert_not_reached ();
      }

      cache_desktop_file (name, mime_type, &load_error);

      if (load_error != NULL)
        {
          g_propagate_error (error, load_error);
          g_strfreev (mime_types);
          return;
        }
    }
  g_strfreev (mime_types);
}

static void
process_desktop_files (const char  *desktop_dir,
                       const char  *prefix,
                       GError     **error)
{
  GError *process_error;
  GDir *dir;
  const char *filename;

  process_error = NULL;
  dir = g_dir_open (desktop_dir, 0, &process_error);

  if (process_error != NULL)
    {
      g_propagate_error (error, process_error);
      return;
    }

  while ((filename = g_dir_read_name (dir)) != NULL)
    {
      char *full_path, *name;

      full_path = g_build_filename (desktop_dir, filename, NULL);

      if (g_file_test (full_path, G_FILE_TEST_IS_DIR))
        {
          char *sub_prefix;

          sub_prefix = g_strdup_printf ("%s%s-", prefix, filename);

          process_desktop_files (full_path, sub_prefix, &process_error);
          g_free (sub_prefix);

          if (process_error != NULL)
            {
              udd_verbose_print (_("Could not process directory \"%s\": %s\n"),
                                 full_path, process_error->message);
              g_error_free (process_error);
              process_error = NULL;
            }
          g_free (full_path);
          continue;
        }
      else if (!g_str_has_suffix (filename, ".desktop"))
        {
          g_free (full_path);
          continue;
        }

      name = g_strdup_printf ("%s%s", prefix, filename);
      process_desktop_file (full_path, name, &process_error);
      g_free (name);

      if (process_error != NULL)
        {
          if (!g_error_matches (process_error,
                                G_KEY_FILE_ERROR,
                                G_KEY_FILE_ERROR_KEY_NOT_FOUND))
            {
              udd_print (_("Could not parse file \"%s\": %s\n"), full_path,
                         process_error->message);
            }
          else
            {
              udd_verbose_print (_("File \"%s\" lacks MimeType key\n"),
                                 full_path);
            }

          g_error_free (process_error);
          process_error = NULL;
        }

      g_free (full_path);
    }

  g_dir_close (dir);
}

static FILE *
open_temp_cache_file (const char *dir, char **filename, GError **error)
{
  int fd;
  char *file;
  FILE *fp;
  mode_t mask;

  file = g_build_filename (dir, TEMP_CACHE_FILENAME_PREFIX, NULL);
  fd = g_mkstemp (file);

  if (fd < 0)
    {
      g_set_error (error, G_FILE_ERROR,
                   g_file_error_from_errno (errno),
                   "%s", g_strerror (errno));
      g_free (file);
      return NULL;
    }

  mask = umask(0);
  (void) umask (mask);

  fchmod (fd, 0666 & ~mask);

  fp = fdopen (fd, "w+");
  if (fp == NULL)
    {
      g_set_error (error, G_FILE_ERROR,
                   g_file_error_from_errno (errno),
                   "%s", g_strerror (errno));
      g_free (file);
      close (fd);
      return NULL;
    }

  if (filename)
    *filename = file;
  else
    g_free (file);

  return fp;
}

static void
add_mime_type (const char *mime_type, GList *desktop_files, FILE *f)
{
  GString *list;
  GList *desktop_file;

  list = g_string_new (mime_type);
  g_string_append_c (list, '=');
  desktop_files = g_list_sort (desktop_files, (GCompareFunc) g_strcmp0);
  for (desktop_file = desktop_files;
       desktop_file != NULL;
       desktop_file = desktop_file->next)
    {
      g_string_append (list, (const char *) desktop_file->data);
      g_string_append_c (list, ';');
    }
  g_string_append_c (list, '\n');

  fputs (list->str, f);

  g_string_free (list, TRUE);
}

static void
sync_database (const char *dir, GError **error)
{
  GError *sync_error;
  char *temp_cache_file, *cache_file;
  FILE *tmp_file;
  GList *keys, *key;

  temp_cache_file = NULL;
  sync_error = NULL;
  tmp_file = open_temp_cache_file (dir, &temp_cache_file, &sync_error);

  if (sync_error != NULL)
    {
      g_propagate_error (error, sync_error);
      return;
    }

  fputs ("[MIME Cache]\n", tmp_file);

  keys = g_hash_table_get_keys (mime_types_map);
  keys = g_list_sort (keys, (GCompareFunc) g_strcmp0);

  for (key = keys; key != NULL; key = key->next)
    add_mime_type (key->data,
                   g_hash_table_lookup (mime_types_map, key->data),
                   tmp_file);

  g_list_free (keys);
  fclose (tmp_file);

  cache_file = g_build_filename (dir, CACHE_FILENAME, NULL);
  if (rename (temp_cache_file, cache_file) < 0)
    {
      g_set_error (error, G_FILE_ERROR,
                   g_file_error_from_errno (errno),
                   _("Cache file \"%s\" could not be written: %s"),
                   cache_file, g_strerror (errno));

      unlink (temp_cache_file);
    }
  g_free (temp_cache_file);
  g_free (cache_file);
}

static void
update_database (const char  *desktop_dir,
                 GError     **error)
{
  GError *update_error;

  mime_types_map = g_hash_table_new_full (g_str_hash, g_str_equal,
                                          (GDestroyNotify)g_free,
                                          NULL);

  update_error = NULL;
  process_desktop_files (desktop_dir, "", &update_error);

  if (update_error != NULL)
    g_propagate_error (error, update_error);
  else
    {
      sync_database (desktop_dir, &update_error);
      if (update_error != NULL)
        g_propagate_error (error, update_error);
    }
  g_hash_table_foreach (mime_types_map, (GHFunc) list_free_deep, NULL);
  g_hash_table_destroy (mime_types_map);
}

static const char **
get_default_search_path (void)
{
  static char **args = NULL;
  const char * const *data_dirs;
  int i;

  if (args != NULL)
    return (const char **) args;

  data_dirs = g_get_system_data_dirs ();

  for (i = 0; data_dirs[i] != NULL; i++);

  args = g_new (char *, i + 1);

  for (i = 0; data_dirs[i] != NULL; i++)
    args[i] = g_build_filename (data_dirs[i], "applications", NULL);

  args[i] = NULL;

  return (const char **) args;
}

void
print_desktop_dirs (const char **dirs)
{
  char *directories;

  directories = g_strjoinv (", ", (char **) dirs);
  udd_verbose_print(_("Search path is now: [%s]\n"), directories);
  g_free (directories);
}

int
main (int    argc,
      char **argv)
{
  GError *error;
  GOptionContext *context;
  const char **desktop_dirs;
  int i;
  gboolean found_processable_dir;

  const GOptionEntry options[] =
   {
     { "quiet", 'q', 0, G_OPTION_ARG_NONE, &quiet,
       N_("Do not display any information about processing and "
          "updating progress"), NULL},

     { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose,
       N_("Display more information about processing and updating progress"),
       NULL},

     { "version", 0, 0, G_OPTION_ARG_NONE, &print_version,
       N_("Show the program version"),
       NULL},

     { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &desktop_dirs,
       NULL, N_("[DIRECTORY...]") },
     { NULL }
   };

#ifdef HAVE_PLEDGE
  if (pledge ("stdio rpath wpath cpath fattr", NULL) == -1) {
    g_printerr ("pledge\n");
    return 1;
  }
#endif

  context = g_option_context_new ("");
  g_option_context_set_summary (context, _("Build cache database of MIME types handled by desktop files."));
  g_option_context_add_main_entries (context, options, NULL);

  desktop_dirs = NULL;
  error = NULL;
  g_option_context_parse (context, &argc, &argv, &error);

  if (error != NULL) {
    g_printerr ("%s\n", error->message);
    g_printerr (_("Run \"%s --help\" to see a full list of available command line options.\n"), argv[0]);
    g_error_free (error);
    return 1;
  }

  if (print_version) {
    g_print("update-desktop-database %s\n", VERSION);
    return 0;
  }

  if (desktop_dirs == NULL || desktop_dirs[0] == NULL)
    desktop_dirs = get_default_search_path ();

  print_desktop_dirs (desktop_dirs);

  found_processable_dir = FALSE;
  for (i = 0; desktop_dirs[i] != NULL; i++)
    {
      error = NULL;
      update_database (desktop_dirs[i], &error);

      if (error != NULL)
        {
          udd_verbose_print (_("Could not create cache file in \"%s\": %s\n"),
                             desktop_dirs[i], error->message);
          g_error_free (error);
          error = NULL;
        }
      else
        found_processable_dir = TRUE;
    }
  g_option_context_free (context);

  if (!found_processable_dir)
    {
      char *directories;

      directories = g_strjoinv (", ", (char **) desktop_dirs);
      udd_print (_("The databases in [%s] could not be updated.\n"),
                 directories);

      g_free (directories);

      return 1;
    }

  return 0;
}
