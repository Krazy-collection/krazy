/* update-desktop-database.c - maintains mimetype<->desktop mapping
 * cache
 *
 * Copyright 2004  Red Hat, Inc. 
 * 
 * Program written by Ray Strode <rstrode@redhat.com>
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

#define NAME "update-desktop-database"
#define CACHE_FILENAME "mimeinfo.cache"
#define TEMP_CACHE_FILENAME_PREFIX ".mimeinfo.cache.XXXXXX"

#define udd_print(...) if (!quiet) g_printerr (__VA_ARGS__)
#define udd_verbose_print(...) if (verbose) g_printerr (__VA_ARGS__)

static FILE *open_temp_cache_file (const char  *dir,
				   char       **filename,
				   GError     **error);
static void add_mime_type (const char *mime_type, GList *desktop_files, FILE *f);
static void sync_database (const char *dir, GError **error);
static void cache_desktop_file (const char  *desktop_file, 
                                const char  *mime_type,
                                GError     **error);
static gboolean is_valid_mime_type (const char *desktop_file, 
                                    const char *mime_type);
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
static gboolean verbose = FALSE, quiet = FALSE;

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

  desktop_files = g_list_prepend (desktop_files, g_strdup (desktop_file));
  g_hash_table_insert (mime_types_map, g_strdup (mime_type), desktop_files);
}


static gboolean
is_valid_mime_type_char (const guchar c)
{
  char invalid_chars [] = "()<>@,;:\\/[]?=\"";
  
  if ((c <= 32) || (c == 127)) 
    {
      /* Filter out control chars and space */
      return FALSE;
    }
  
  if (memchr (invalid_chars, c, sizeof (invalid_chars)) != NULL) 
    {
      return FALSE;
    }
  
  return TRUE;
}


static gboolean
is_valid_mime_type (const char *desktop_file, 
                    const char *mime_type)
{
  gulong subtype_offset;
  gulong valid_chars;
  
  valid_chars = 0;
  subtype_offset = 0;
  
  while (mime_type[valid_chars] != '\0') 
    {
      if (mime_type[valid_chars] == '/') 
	{
	  if (valid_chars == 0) 
	    {
	      /* We encountered a / before any valid char */
              udd_print ("File '%s' contains invalid MIME type '%s' "
                         "that starts with a slash\n",
                         desktop_file, mime_type);
	      return FALSE;
	    }
	  if (subtype_offset != 0) 
	    {
	      /* We already encountered a '/' previously */
              udd_print ("File '%s' contains invalid MIME type '%s' "
                         "that has more than one slash\n", 
                         desktop_file, mime_type);
              return FALSE;
            }
          subtype_offset = valid_chars;
        } 
      else if (!is_valid_mime_type_char (mime_type[valid_chars])) 
        {
          udd_print ("File '%s' contains invalid MIME type '%s' "
                     "that contains invalid characters\n", 
                     desktop_file, mime_type);
          return FALSE;
        }

      valid_chars++;			    
    }

  if (subtype_offset == 0) 
    {
      /* The mime type didn't contain any / */
      udd_print ("File '%s' contains invalid MIME type '%s' that is "
                 "missing a slash\n", desktop_file, mime_type);
      return FALSE;
    }

  if ((subtype_offset != 0) && (subtype_offset == valid_chars)) 
    {
      /* Missing subtype name */
      udd_print ("File '%s' contains invalid MIME type '%s' that is "
                 "missing a subtype\n", desktop_file, mime_type);
      return FALSE;
    }
  
  return TRUE;
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

  mime_types = g_key_file_get_string_list (keyfile,
                                           g_key_file_get_start_group (keyfile),
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

      mime_type = g_strchomp (mime_types[i]);
      if (!is_valid_mime_type (desktop_file, mime_types[i])) 
	continue;

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
              udd_verbose_print ("Could not process directory '%s':\n"
                         "\t%s\n", full_path, process_error->message);
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
              udd_print ("Could not parse file '%s': %s\n", full_path,
                         process_error->message);
            }
          else
            {
              udd_verbose_print ("File '%s' lacks MimeType key\n", full_path);
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
  for (desktop_file = desktop_files;
       desktop_file != NULL; 
       desktop_file = desktop_file->next)
    {
      g_string_append (list, (const char *) desktop_file->data);

      if (desktop_files->next != NULL)
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

  temp_cache_file = NULL;
  sync_error = NULL;
  tmp_file = open_temp_cache_file (dir, &temp_cache_file, &sync_error);

  if (sync_error != NULL)
    {
      g_propagate_error (error, sync_error);
      return;
    }

  fputs ("[MIME Cache]\n", tmp_file);
  g_hash_table_foreach (mime_types_map, (GHFunc) add_mime_type, tmp_file);

  fclose (tmp_file);

  cache_file = g_build_filename (dir, CACHE_FILENAME, NULL);
  if (rename (temp_cache_file, cache_file) < 0)
    {
      g_set_error (error, G_FILE_ERROR,
                   g_file_error_from_errno (errno),
                   _("Cache file '%s' could not be written: %s"),
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
  int i;
  const char *delimiter;

  udd_verbose_print(_("Search path is now: ["));
  delimiter = "";
  for (i = 0; dirs[i] != NULL; i++)
    {
      udd_verbose_print (delimiter);
      delimiter = ", ";
      udd_verbose_print (dirs[i]);
    }
  udd_verbose_print ("]\n");
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
     { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose,
       N_("Display more information about processing and updating progress"),
       NULL},

     { "quiet", 'q', 0, G_OPTION_ARG_NONE, &quiet,
       N_("Don't display any information about about processing and "
          "updating progress"), NULL},

     { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &desktop_dirs,
       NULL, N_("[DIRECTORY...]") },
     { NULL }
   };

  context = g_option_context_new ("");
  g_option_context_add_main_entries (context, options, NULL);

  desktop_dirs = NULL;
  error = NULL;
  g_option_context_parse (context, &argc, &argv, &error);

  if (error != NULL) {
	  g_printerr ("%s\n", error->message);
	  g_printerr (_("Run '%s --help' to see a full list of available command line options.\n"), argv[0]);
	  g_error_free (error);
	  return 1;
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
          udd_verbose_print (_("Could not create cache file in directory '%s':"
                               "\n\t%s\n"),
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
      udd_print (_("No directories in update-desktop-database search path "
                   "could be processed and updated.\n"));
      return 1;
    }

  return 0;
}
