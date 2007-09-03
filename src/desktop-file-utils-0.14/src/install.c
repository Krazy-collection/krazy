
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

static const char** args = NULL;
static gboolean delete_original = FALSE;
static gboolean copy_generic_name_to_name = FALSE;
static gboolean copy_name_to_generic_name = FALSE;
static gboolean rebuild_mime_info_cache = FALSE;
static char *vendor_name = NULL;
static char *target_dir = NULL;
static GSList *added_categories = NULL;
static GSList *removed_categories = NULL;
static GSList *added_only_show_in = NULL;
static GSList *removed_only_show_in = NULL;
static GSList *removed_keys = NULL;
static GSList *added_mime_types = NULL;
static GSList *removed_mime_types = NULL;
static mode_t permissions = 0644;

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
  char *dirname;
  char *basename;
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

  if (copy_name_to_generic_name)
    dfu_key_file_copy_key (kf, GROUP_DESKTOP_ENTRY, "Name",
                               GROUP_DESKTOP_ENTRY, "GenericName");

  if (copy_generic_name_to_name)
    dfu_key_file_copy_key (kf, GROUP_DESKTOP_ENTRY, "GenericName",
                               GROUP_DESKTOP_ENTRY, "Name");
  
  /* Mark file as having been processed by us, so automated
   * tools can check that desktop files went through our
   * munging
   */
  g_key_file_set_string (kf, GROUP_DESKTOP_ENTRY,
                         "X-Desktop-File-Install-Version", VERSION);

#define PROCESS_LIST(key, added, removed)                       \
  /* Add to the list */                                         \
  tmp = added;                                                  \
  while (tmp != NULL)                                           \
    {                                                           \
      dfu_key_file_merge_list (kf, GROUP_DESKTOP_ENTRY,         \
                               key, tmp->data);                 \
      tmp = tmp->next;                                          \
    }                                                           \
                                                                \
  /* Remove from the list */                                    \
  tmp = removed;                                                \
  while (tmp != NULL)                                           \
    {                                                           \
      dfu_key_file_remove_list (kf, GROUP_DESKTOP_ENTRY,        \
                                key, tmp->data);                \
      tmp = tmp->next;                                          \
    }

  /* Add/remove categories */
  PROCESS_LIST ("Categories", added_categories, removed_categories);

  /* Add/remove onlyshowin */
  PROCESS_LIST ("OnlyShowIn", added_only_show_in, removed_only_show_in);

  /* Remove keys */
  tmp = removed_keys;
  while (tmp != NULL)
    {
      g_key_file_remove_key (kf, GROUP_DESKTOP_ENTRY, tmp->data, NULL);

      tmp = tmp->next;
    }

  /* Add/remove mime-types */
  PROCESS_LIST ("MimeType", added_mime_types, removed_mime_types);


  dirname = g_path_get_dirname (filename);
  basename = g_path_get_basename (filename);
  
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

  g_free (dirname);
  g_free (basename);
  
  if (!dfu_key_file_to_file (kf, new_filename, err)) {
    g_key_file_free (kf);
    g_free (new_filename);
    return;
  }
  
  g_key_file_free (kf);

  /* Load and validate the file we just wrote */
  if (!desktop_file_validate (new_filename, FALSE, TRUE))
    {
      g_set_error (err, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_PARSE,
                   _("Failed to validate the created desktop file"));
      g_unlink (new_filename);
      g_free (new_filename);
      return;
    }

  if (g_chmod (new_filename, permissions) < 0)
    {
      g_set_error (err, G_FILE_ERROR,
                   g_file_error_from_errno (errno),
                   _("Failed to set permissions %o on \"%s\": %s"),
                   permissions, new_filename, g_strerror (errno));

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

  if (rebuild_mime_info_cache)
    {
      rebuild_error = NULL;
      rebuild_cache (target_dir, &rebuild_error);

      if (rebuild_error != NULL)
        g_propagate_error (err, rebuild_error);
    }
  
  g_free (new_filename);
}

static gboolean parse_options_callback (const gchar  *option_name,
                                        const gchar  *value,
                                        gpointer      data,
                                        GError      **error);


static const GOptionEntry options[] = {
  {
#define OPTION_VENDOR "vendor"
    OPTION_VENDOR,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify the vendor prefix to be applied to the desktop file. If the file already has this prefix, nothing happens."),
    NULL
  },
  {
#define OPTION_DIR "dir"
    OPTION_DIR,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify the directory where files should be installed."),
    NULL
  },
  {
    "delete-original",
    '\0',
    '\0',
    G_OPTION_ARG_NONE,
    &delete_original,
    N_("Delete the source desktop file, leaving only the target file. Effectively \"renames\" a desktop file."),
    NULL
  },
  {
#define OPTION_MODE "mode"
    OPTION_MODE,
    'm',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Set the given permissions on the destination file."),
    NULL
  },
  {
    "rebuild-mime-info-cache",
    '\0',
    '\0',
    G_OPTION_ARG_NONE,
    &rebuild_mime_info_cache,
    N_("After installing desktop file rebuild the mime-types application database."),
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

static const GOptionEntry edit_options[] = {
  {
#define OPTION_ADD_CATEGORY "add-category"
    OPTION_ADD_CATEGORY,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify a category to be added to the Categories field."),
    N_("CATEGORY")
  },
  {
#define OPTION_REMOVE_CATEGORY "remove-category"
    OPTION_REMOVE_CATEGORY,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify a category to be removed from the Categories field."),
    N_("CATEGORY")
  },
  {
#define OPTION_ADD_ONLY_SHOW_IN "add-only-show-in"
    OPTION_ADD_ONLY_SHOW_IN,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify a product name to be added to the OnlyShowIn field."),
    N_("PRODUCT")
  },
  {
#define OPTION_REMOVE_ONLY_SHOW_IN "remove-only-show-in"
    OPTION_REMOVE_ONLY_SHOW_IN,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify a product name to be removed from the OnlyShowIn field."),
    N_("PRODUCT")
  },
  {
    "copy-name-to-generic-name",
    '\0',
    '\0',
    G_OPTION_ARG_NONE,
    &copy_name_to_generic_name,
    N_("Copy the contents of the \"Name\" field to the \"GenericName\" field."),
    NULL
  },
  {
    "copy-generic-name-to-name",
    '\0',
    '\0',
    G_OPTION_ARG_NONE,
    &copy_generic_name_to_name,
    N_("Copy the contents of the \"GenericName\" field to the \"Name\" field."),
    NULL
  },
  {
#define OPTION_REMOVE_KEY "remove-key"
    OPTION_REMOVE_KEY,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify a field to be removed from the desktop file."),
    N_("KEY")
  },
  {
#define OPTION_ADD_MIME_TYPE "add-mime-type"
    OPTION_ADD_MIME_TYPE,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify a mime-type to be added to the MimeType field."),
    N_("MIME-TYPE")
  },
  {
#define OPTION_REMOVE_MIME_TYPE "remove-mime-type"
    OPTION_REMOVE_MIME_TYPE,
    '\0',
    '\0',
    G_OPTION_ARG_CALLBACK,
    parse_options_callback,
    N_("Specify a mime-type to be removed from the MimeType field."),
    N_("MIME-TYPE")
  },
  {
    NULL
  }
};

static gboolean
parse_options_callback (const gchar  *option_name,
                        const gchar  *value,
                        gpointer      data,
                        GError      **error)
{
  char **list;
  int    i;

  /* skip "-" or "--" */
  option_name++;
  if (*option_name == '-')
    option_name++;

  if (strcmp (OPTION_VENDOR, option_name) == 0)
    {
      if (vendor_name)
        {
          g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                       _("Can only specify --vendor once"));
          return FALSE;
        }
      
      vendor_name = g_strdup (value);
    }

  else if (strcmp (OPTION_DIR, option_name) == 0)
    {
      if (target_dir)
        {
          g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                       _("Can only specify --dir once"));
          return FALSE;
        }

      target_dir = g_strdup (value);
    }

#define PARSE_OPTION_LIST(glist)                                        \
  do {                                                                  \
    list = g_strsplit (value, ";", 0);                                  \
    for (i = 0; list[i]; i++)                                           \
      {                                                                 \
        if (*list[i] == '\0')                                           \
          continue;                                                     \
                                                                        \
        glist = g_slist_prepend (glist, g_strdup (list[i]));            \
      }                                                                 \
  } while (0)

  else if (strcmp (OPTION_ADD_CATEGORY, option_name) == 0)
    {
      PARSE_OPTION_LIST (added_categories);
    }

  else if (strcmp (OPTION_REMOVE_CATEGORY, option_name) == 0)
    {
      PARSE_OPTION_LIST (removed_categories);
    }

  else if (strcmp (OPTION_ADD_ONLY_SHOW_IN, option_name) == 0)
    {
      PARSE_OPTION_LIST (added_only_show_in);
    }

  else if (strcmp (OPTION_REMOVE_ONLY_SHOW_IN, option_name) == 0)
    {
      PARSE_OPTION_LIST (removed_only_show_in);
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

  else if (strcmp (OPTION_REMOVE_KEY, option_name) == 0)
    {
      removed_keys = g_slist_prepend (removed_keys,
                                      g_strdup (value));
    }

  else if (strcmp (OPTION_ADD_MIME_TYPE, option_name) == 0)
    {
      PARSE_OPTION_LIST (added_mime_types);
    }

  else if (strcmp (OPTION_REMOVE_MIME_TYPE, option_name) == 0)
    {
      PARSE_OPTION_LIST (removed_mime_types);
    }

  else
    {
        g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
                     _("Unknown option \"%s\""), option_name);
        
        return FALSE;
    }

  return TRUE;
}

int
main (int argc, char **argv)
{
  GOptionContext *context;
  GOptionGroup *group;
  GError* err = NULL;
  int i;
  mode_t dir_permissions;
  
  setlocale (LC_ALL, "");
  
  context = g_option_context_new ("");
  g_option_context_add_main_entries (context, options, NULL);

  group = g_option_group_new ("edit", _("Edition options for desktop file"), _("Show desktop file edition options"), NULL, NULL);
  g_option_group_add_entries (group, edit_options);
  g_option_context_add_group (context, group);

  err = NULL;
  g_option_context_parse (context, &argc, &argv, &err);

  if (err != NULL) {
	  g_printerr ("%s\n", err->message);
	  g_printerr (_("Run '%s --help' to see a full list of available command line options.\n"), argv[0]);
	  g_error_free (err);
	  return 1;
  }

  if (vendor_name == NULL && g_getenv ("DESKTOP_FILE_VENDOR"))
    vendor_name = g_strdup (g_getenv ("DESKTOP_FILE_VENDOR"));
  
  if (copy_generic_name_to_name && copy_name_to_generic_name)
    {
      g_printerr (_("Specifying both --copy-name-to-generic-name and --copy-generic-name-to-name at once doesn't make much sense.\n"));
      return 1;
    }
  
  if (target_dir == NULL && g_getenv ("DESKTOP_FILE_INSTALL_DIR"))
    target_dir = g_strdup (g_getenv ("DESKTOP_FILE_INSTALL_DIR"));

  if (target_dir == NULL)
    target_dir = g_build_filename (DATADIR, "applications", NULL);

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
  
  i = 0;
  while (args && args[i])
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
      
      ++i;
    }

  if (i == 0)
    {
      g_printerr (_("Must specify one or more desktop files to install\n"));

      return 1;
    }
  
  g_option_context_free (context);

  return 0;
}
