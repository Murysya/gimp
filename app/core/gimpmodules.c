/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpmodules.c
 * (C) 1999 Austin Donnelly <austin@gimp.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stdio.h>
#include <string.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <time.h>

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpmodule/gimpmodule.h"

#include "core-types.h"

#include "gimp.h"
#include "gimpcoreconfig.h"
#include "gimpdatafiles.h"
#include "gimplist.h"
#include "gimpmodules.h"

#include "gimprc.h"

#include "libgimp/gimpintl.h"


#define DUMP_DB 1


static void         gimp_modules_module_initialize   (const gchar *filename,
                                                      gpointer     loader_data);

static GimpModule * gimp_modules_module_find_by_path (Gimp        *gimp,
                                                      const char  *fullpath);

#ifdef DUMP_DB
static void         gimp_modules_dump_module         (gpointer     data,
                                                      gpointer     user_data);
#endif

static gboolean     gimp_modules_write_modulerc      (Gimp        *gimp);

static void         gimp_modules_module_on_disk_func (gpointer     data, 
                                                      gpointer     user_data);
static void         gimp_modules_module_remove_func  (gpointer     data, 
                                                      gpointer     user_data);


void
gimp_modules_init (Gimp *gimp)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));

  gimp->modules = gimp_list_new (GIMP_TYPE_MODULE,
                                 GIMP_CONTAINER_POLICY_STRONG);
  gimp_object_set_name (GIMP_OBJECT (gimp->modules), "modules");

  gimp->write_modulerc = FALSE;
}

void
gimp_modules_exit (Gimp *gimp)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));

  if (gimp->modules)
    {
      g_object_unref (G_OBJECT (gimp->modules));
      gimp->modules = NULL;
    }
}

void
gimp_modules_load (Gimp *gimp)
{
  gchar *filename;

  g_return_if_fail (GIMP_IS_GIMP (gimp));

  filename = gimp_personal_rc_file ("modulerc");
  gimprc_parse_file (filename);
  g_free (filename);

  if (g_module_supported ())
    gimp_datafiles_read_directories (gimp->config->module_path,
                                     0 /* no flags */,
				     gimp_modules_module_initialize,
                                     gimp);

#ifdef DUMP_DB
  gimp_container_foreach (gimp->modules, gimp_modules_dump_module, NULL);
#endif
}

void
gimp_modules_unload (Gimp *gimp)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));

  if (gimp->write_modulerc)
    {
      if (gimp_modules_write_modulerc (gimp))
	{
	  gimp->write_modulerc = FALSE;
	}
    }
}

void
gimp_modules_refresh (Gimp *gimp)
{
  GList *kill_list = NULL;

  g_return_if_fail (GIMP_IS_GIMP (gimp));

  /* remove modules we don't have on disk anymore */
  gimp_container_foreach (gimp->modules,
                          gimp_modules_module_on_disk_func,
                          &kill_list);
  g_list_foreach (kill_list,
                  gimp_modules_module_remove_func,
                  gimp);
  g_list_free (kill_list);

  /* walk filesystem and add new things we find */
  gimp_datafiles_read_directories (gimp->config->module_path,
                                   0 /* no flags */,
				   gimp_modules_module_initialize,
                                   gimp);
}

static void
add_to_inhibit_string (gpointer data, 
		       gpointer user_data)
{
  GimpModule *module = data;
  GString    *str    = user_data;

  if (module->load_inhibit)
    {
      str = g_string_append_c (str, G_SEARCHPATH_SEPARATOR);
      str = g_string_append (str, module->filename);
    }
}

static gboolean
gimp_modules_write_modulerc (Gimp *gimp)
{
  GString  *str;
  gchar    *p;
  gchar    *filename;
  FILE     *fp;
  gboolean  saved = FALSE;

  str = g_string_new (NULL);
  gimp_container_foreach (gimp->modules, add_to_inhibit_string, str);
  if (str->len > 0)
    p = str->str + 1;
  else
    p = "";

  filename = gimp_personal_rc_file ("modulerc");
  fp = fopen (filename, "wt");
  g_free (filename);
  if (fp)
    {
      fprintf (fp, "(module-load-inhibit \"%s\")\n", p);
      fclose (fp);
      saved = TRUE;
    }

  g_string_free (str, TRUE);

  return saved;
}

/* name must be of the form lib*.so (Unix) or *.dll (Win32) */
static gboolean
valid_module_name (const gchar *filename)
{
  gchar *basename;
  gint   len;

  basename = g_path_get_basename (filename);

  len = strlen (basename);

#if !defined(G_OS_WIN32) && !defined(G_WITH_CYGWIN) && !defined(__EMX__)
  if (len < 3 + 1 + 3)
    goto no_module;

  if (strncmp (basename, "lib", 3))
    goto no_module;

  if (strcmp (basename + len - 3, ".so"))
    goto no_module;
#else
  if (len < 1 + 4)
    goto no_module;

  if (g_strcasecmp (basename + len - 4, ".dll"))
    goto no_module;
#endif

  g_free (basename);

  return TRUE;

 no_module:
  g_free (basename);

  return FALSE;
}

static void
gimp_modules_module_initialize (const gchar *filename,
                                gpointer     loader_data)
{
  GimpModule *module;
  Gimp       *gimp;

  gimp = GIMP (loader_data);

  if (! valid_module_name (filename))
    return;

  /* don't load if we already know about it */
  if (gimp_modules_module_find_by_path (gimp, filename))
    return;

  module = gimp_module_new (filename,
                            gimp->config->module_db_load_inhibit,
                            gimp->be_verbose);

  gimp_container_add (gimp->modules, (GimpObject *) module);
}

static GimpModule *
gimp_modules_module_find_by_path (Gimp       *gimp,
                                  const char *fullpath)
{
  GimpModule *module;
  GList      *list;

  for (list = GIMP_LIST (gimp->modules)->list; list; list = g_list_next (list))
    {
      module = (GimpModule *) list->data;

      if (! strcmp (module->filename, fullpath))
        return module;
    }

  return NULL;
}

#ifdef DUMP_DB
static void
gimp_modules_dump_module (gpointer data, 
                          gpointer user_data)
{
  GimpModule *i = data;

  g_print ("\n%s: %i\n",
	   i->filename,
           i->state /* statename[i->state] */);

  g_print ("  module:%p  lasterr:%s  query:%p register:%p\n",
	   i->module,
           i->last_module_error ? i->last_module_error : "NONE",
	   i->query_module,
	   i->register_module);

  if (i->info)
    {
      g_print ("  purpose:   %s\n"
               "  author:    %s\n"
               "  version:   %s\n"
               "  copyright: %s\n"
               "  date:      %s\n",
               i->info->purpose   ? i->info->purpose   : "NONE",
               i->info->author    ? i->info->author    : "NONE",
               i->info->version   ? i->info->version   : "NONE",
               i->info->copyright ? i->info->copyright : "NONE",
               i->info->date      ? i->info->date      : "NONE");
    }
}
#endif

static void
gimp_modules_module_on_disk_func (gpointer data, 
                                  gpointer user_data)
{
  GimpModule  *module;
  GList      **kill_list;
  gint         old_on_disk;

  module    = (GimpModule *) data;
  kill_list = (GList **) user_data;

  old_on_disk = module->on_disk;

  module->on_disk = g_file_test (module->filename, G_FILE_TEST_IS_REGULAR);

  /* if it's not on the disk, and it isn't in memory, mark it to be
   * removed later.
   */
  if (! module->on_disk && ! module->module)
    {
      *kill_list = g_list_append (*kill_list, module);
      module = NULL;
    }

  if (module && module->on_disk != old_on_disk)
    gimp_module_modified (module);
}

static void
gimp_modules_module_remove_func (gpointer data, 
                                 gpointer user_data)
{
  GimpModule *module;
  Gimp       *gimp;

  module = (GimpModule *) data;
  gimp   = (Gimp *) user_data;

  gimp_container_remove (gimp->modules, (GimpObject *) module);
}
