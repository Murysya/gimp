/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is autogenerated by pdbgen.pl. */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpbase/gimpprotocol.h"

#include "pdb-types.h"
#include "procedural_db.h"

#include "core/gimp.h"
#include "plug-in/plug-in-def.h"
#include "plug-in/plug-in-params.h"
#include "plug-in/plug-in-proc-def.h"
#include "plug-in/plug-in.h"
#include "plug-in/plug-ins.h"

#ifdef HAVE_GLIBC_REGEX
#include <regex.h>
#else
#include "regexrepl/regex.h"
#endif

static ProcRecord plugins_query_proc;
static ProcRecord plugin_domain_register_proc;
static ProcRecord plugin_help_register_proc;
static ProcRecord plugin_menu_register_proc;
static ProcRecord plugin_icon_register_proc;

void
register_plug_in_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &plugins_query_proc);
  procedural_db_register (gimp, &plugin_domain_register_proc);
  procedural_db_register (gimp, &plugin_help_register_proc);
  procedural_db_register (gimp, &plugin_menu_register_proc);
  procedural_db_register (gimp, &plugin_icon_register_proc);
}

static int
match_strings (regex_t *preg,
               gchar   *a)
{
  return regexec (preg, a, 0, NULL, 0);
}

static Argument *
plugins_query_invoker (Gimp         *gimp,
                       GimpContext  *context,
                       GimpProgress *progress,
                       Argument     *args)
{
  Argument *return_args;
  gchar *search_str;
  gint32 num_plugins = 0;
  gchar **menu_strs;
  gchar **accel_strs;
  gchar **prog_strs;
  gchar **types_strs;
  gint32 *time_ints;
  gchar **realname_strs;
  GSList *list;
  gint i = 0;
  regex_t sregex;

  search_str = (gchar *) args[0].value.pdb_pointer;

  if (search_str && strlen (search_str))
    regcomp (&sregex, search_str, REG_ICASE);
  else
    search_str = NULL;

  /* count number of plugin entries, then allocate arrays of correct size
   * where we can store the strings.
   */

  for (list = gimp->plug_in_proc_defs; list; list = g_slist_next (list))
    {
      PlugInProcDef *proc_def = list->data;

      if (proc_def->prog && proc_def->menu_paths)
        {
          gchar *name;

          if (proc_def->menu_label)
            {
              name = proc_def->menu_label;
            }
          else
            {
              name = strrchr (proc_def->menu_paths->data, '/');

              if (name)
                name = name + 1;
              else
                name = proc_def->menu_paths->data;
            }

          if (search_str && match_strings (&sregex, name))
            continue;

          num_plugins++;
        }
    }

  menu_strs     = g_new (gchar *, num_plugins);
  accel_strs    = g_new (gchar *, num_plugins);
  prog_strs     = g_new (gchar *, num_plugins);
  types_strs    = g_new (gchar *, num_plugins);
  realname_strs = g_new (gchar *, num_plugins);
  time_ints     = g_new (gint   , num_plugins);

  for (list = gimp->plug_in_proc_defs; list; list = g_slist_next (list))
    {
      PlugInProcDef *proc_def = list->data;

      if (i > num_plugins)
        g_error ("Internal error counting plugins");

      if (proc_def->prog && proc_def->menu_paths)
        {
          ProcRecord *pr = &proc_def->db_info;
          gchar      *name;

          if (proc_def->menu_label)
            {
              name = proc_def->menu_label;
            }
          else
            {
              name = strrchr (proc_def->menu_paths->data, '/');

              if (name)
                name = name + 1;
              else
                name = proc_def->menu_paths->data;
            }

          if (search_str && match_strings (&sregex, name))
            continue;

          menu_strs[i]     = gimp_strip_uline (proc_def->menu_paths->data);
          accel_strs[i]    = NULL;
          prog_strs[i]     = g_strdup (proc_def->prog);
          types_strs[i]    = g_strdup (proc_def->image_types);
          realname_strs[i] = g_strdup (pr->name);
          time_ints[i]     = proc_def->mtime;

          i++;
        }
    }

  if (search_str)
    regfree (&sregex);

  return_args = procedural_db_return_args (&plugins_query_proc, TRUE);

  return_args[1].value.pdb_int = num_plugins;
  return_args[2].value.pdb_pointer = menu_strs;
  return_args[3].value.pdb_int = num_plugins;
  return_args[4].value.pdb_pointer = accel_strs;
  return_args[5].value.pdb_int = num_plugins;
  return_args[6].value.pdb_pointer = prog_strs;
  return_args[7].value.pdb_int = num_plugins;
  return_args[8].value.pdb_pointer = types_strs;
  return_args[9].value.pdb_int = num_plugins;
  return_args[10].value.pdb_pointer = time_ints;
  return_args[11].value.pdb_int = num_plugins;
  return_args[12].value.pdb_pointer = realname_strs;

  return return_args;
}

static ProcArg plugins_query_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "search_string",
    "If not an empty string then use this as a search pattern"
  }
};

static ProcArg plugins_query_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "menu_path",
    "The menu path of the plugin"
  },
  {
    GIMP_PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "plugin_accelerator",
    "String representing keyboard accelerator (could be empty string)"
  },
  {
    GIMP_PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "plugin_location",
    "Location of the plugin program"
  },
  {
    GIMP_PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "plugin_image_type",
    "Type of image that this plugin will work on"
  },
  {
    GIMP_PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    GIMP_PDB_INT32ARRAY,
    "plugin_install_time",
    "Time that the plugin was installed"
  },
  {
    GIMP_PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "plugin_real_name",
    "The internal name of the plugin"
  }
};

static ProcRecord plugins_query_proc =
{
  "gimp_plugins_query",
  "Queries the plugin database for its contents.",
  "This procedure queries the contents of the plugin database.",
  "Andy Thomas",
  "Andy Thomas",
  "1998",
  GIMP_INTERNAL,
  1,
  plugins_query_inargs,
  12,
  plugins_query_outargs,
  { { plugins_query_invoker } }
};

static Argument *
plugin_domain_register_invoker (Gimp         *gimp,
                                GimpContext  *context,
                                GimpProgress *progress,
                                Argument     *args)
{
  gboolean success = TRUE;
  gchar *domain_name;
  gchar *domain_path;

  domain_name = (gchar *) args[0].value.pdb_pointer;
  if (domain_name == NULL || !g_utf8_validate (domain_name, -1, NULL))
    success = FALSE;

  domain_path = (gchar *) args[1].value.pdb_pointer;

  if (success)
    {
      if (gimp->current_plug_in && gimp->current_plug_in->query)
        {
          plug_in_def_set_locale_domain_name (gimp->current_plug_in->plug_in_def,
                                              domain_name);
          plug_in_def_set_locale_domain_path (gimp->current_plug_in->plug_in_def,
                                              domain_path);
        }
      else
        success = FALSE;
    }

  return procedural_db_return_args (&plugin_domain_register_proc, success);
}

static ProcArg plugin_domain_register_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "domain_name",
    "The name of the textdomain (must be unique)"
  },
  {
    GIMP_PDB_STRING,
    "domain_path",
    "The absolute path to the compiled message catalog (may be NULL)"
  }
};

static ProcRecord plugin_domain_register_proc =
{
  "gimp_plugin_domain_register",
  "Registers a textdomain for localisation.",
  "This procedure adds a textdomain to the list of domains Gimp searches for strings when translating its menu entries. There is no need to call this function for plug-ins that have their strings included in the gimp-std-plugins domain as that is used by default. If the compiled message catalog is not in the standard location, you may specify an absolute path to another location. This procedure can only be called in the query function of a plug-in and it has to be called before any procedure is installed.",
  "Sven Neumann",
  "Sven Neumann",
  "2000",
  GIMP_INTERNAL,
  2,
  plugin_domain_register_inargs,
  0,
  NULL,
  { { plugin_domain_register_invoker } }
};

static Argument *
plugin_help_register_invoker (Gimp         *gimp,
                              GimpContext  *context,
                              GimpProgress *progress,
                              Argument     *args)
{
  gboolean success = TRUE;
  gchar *domain_name;
  gchar *domain_uri;

  domain_name = (gchar *) args[0].value.pdb_pointer;
  if (domain_name == NULL || !g_utf8_validate (domain_name, -1, NULL))
    success = FALSE;

  domain_uri = (gchar *) args[1].value.pdb_pointer;
  if (domain_uri == NULL || !g_utf8_validate (domain_uri, -1, NULL))
    success = FALSE;

  if (success)
    {
      if (gimp->current_plug_in && gimp->current_plug_in->query)
        {
          plug_in_def_set_help_domain_name (gimp->current_plug_in->plug_in_def,
                                            domain_name);
          plug_in_def_set_help_domain_uri (gimp->current_plug_in->plug_in_def,
                                           domain_uri);
        }
      else
        success = FALSE;
    }

  return procedural_db_return_args (&plugin_help_register_proc, success);
}

static ProcArg plugin_help_register_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "domain_name",
    "The XML namespace of the plug-in's help pages"
  },
  {
    GIMP_PDB_STRING,
    "domain_uri",
    "The root URI of the plug-in's help pages"
  }
};

static ProcRecord plugin_help_register_proc =
{
  "gimp_plugin_help_register",
  "Register a help path for a plug-in.",
  "This procedure changes the help rootdir for the plug-in which calls it. All subsequent calls of gimp_help from this plug-in will be interpreted relative to this rootdir.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer <mitch@gimp.org>",
  "2000",
  GIMP_INTERNAL,
  2,
  plugin_help_register_inargs,
  0,
  NULL,
  { { plugin_help_register_invoker } }
};

static Argument *
plugin_menu_register_invoker (Gimp         *gimp,
                              GimpContext  *context,
                              GimpProgress *progress,
                              Argument     *args)
{
  gboolean success = TRUE;
  gchar *procedure_name;
  gchar *menu_path;

  procedure_name = (gchar *) args[0].value.pdb_pointer;
  if (procedure_name == NULL || !g_utf8_validate (procedure_name, -1, NULL))
    success = FALSE;

  menu_path = (gchar *) args[1].value.pdb_pointer;
  if (menu_path == NULL || !g_utf8_validate (menu_path, -1, NULL))
    success = FALSE;

  if (success)
    {
      if (gimp->current_plug_in && gimp->current_plug_in->query)
        {
          GSList *list;

          for (list = gimp->current_plug_in->plug_in_def->proc_defs;
               list;
               list = g_slist_next (list))
            {
              PlugInProcDef *proc_def = list->data;

              if (! strcmp (procedure_name, proc_def->db_info.name))
                {
                  if (proc_def->menu_label)
                    {
                      GError *error = NULL;

                      if (! plug_in_proc_args_check (gimp->current_plug_in->name,
                                                     gimp->current_plug_in->prog,
                                                     procedure_name,
                                                     menu_path,
                                                     proc_def->db_info.args,
                                                     proc_def->db_info.num_args,
                                                     proc_def->db_info.values,
                                                     proc_def->db_info.num_values,
                                                     &error))
                        {
                          g_message (error->message);
                          g_clear_error (&error);

                          success = FALSE;
                        }
                      else
                        {
                          proc_def->menu_paths = g_list_append (proc_def->menu_paths,
                                                                g_strdup (menu_path));
                        }
                    }
                  else
                    {
                      g_message ("Plug-In \"%s\"\n(%s)\n\n"
                                 "attempted to install additional menu_path \"%s\"\n"
                                 "for procedure \"%s\".\n"
                                 "However the menu_path given in "
                                 "gimp_install_procedure() already contained "
                                 "a path. To make this work, pass just the menu's "
                                 "label to gimp_install_procedure().",
                                 gimp_filename_to_utf8 (gimp->current_plug_in->name),
                                 gimp_filename_to_utf8 (gimp->current_plug_in->prog),
                                 menu_path, procedure_name);

                      success = FALSE;
                    }

                  break;
                }
            }

          if (! list)
            success = FALSE;
        }
      else
        success = FALSE;
    }

  return procedural_db_return_args (&plugin_menu_register_proc, success);
}

static ProcArg plugin_menu_register_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "procedure_name",
    "The procedure for which to install the menu path"
  },
  {
    GIMP_PDB_STRING,
    "menu_path",
    "The procedure's additional menu path"
  }
};

static ProcRecord plugin_menu_register_proc =
{
  "gimp_plugin_menu_register",
  "Register an additional menu path for a plug-in procedure.",
  "This procedure installs an additional menu entry for the given procedure.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer <mitch@gimp.org>",
  "2004",
  GIMP_INTERNAL,
  2,
  plugin_menu_register_inargs,
  0,
  NULL,
  { { plugin_menu_register_invoker } }
};

static Argument *
plugin_icon_register_invoker (Gimp         *gimp,
                              GimpContext  *context,
                              GimpProgress *progress,
                              Argument     *args)
{
  gboolean success = TRUE;
  gchar *procedure_name;
  gint32 icon_type;
  gint32 icon_data_length;
  guint8 *icon_data;

  procedure_name = (gchar *) args[0].value.pdb_pointer;
  if (procedure_name == NULL || !g_utf8_validate (procedure_name, -1, NULL))
    success = FALSE;

  icon_type = args[1].value.pdb_int;
  if (icon_type < GIMP_ICON_TYPE_STOCK_ID || icon_type > GIMP_ICON_TYPE_IMAGE_FILE)
    success = FALSE;

  icon_data_length = args[2].value.pdb_int;
  if (icon_data_length <= 0)
    success = FALSE;

  icon_data = (guint8 *) args[3].value.pdb_pointer;

  if (success)
    {
      if (gimp->current_plug_in && gimp->current_plug_in->query)
        {
          GSList *list;

          for (list = gimp->current_plug_in->plug_in_def->proc_defs;
               list;
               list = g_slist_next (list))
            {
              PlugInProcDef *proc_def = list->data;

              if (! strcmp (procedure_name, proc_def->db_info.name))
                {
                  if (proc_def->icon_data)
                    {
                      g_free (proc_def->icon_data);
                      proc_def->icon_data_length = -1;
                      proc_def->icon_data        = NULL;
                    }

                  proc_def->icon_type = icon_type;

                  switch (proc_def->icon_type)
                    {
                    case GIMP_ICON_TYPE_STOCK_ID:
                    case GIMP_ICON_TYPE_IMAGE_FILE:
                      proc_def->icon_data_length = -1;
                      proc_def->icon_data        = g_strdup (icon_data);
                      break;

                    case GIMP_ICON_TYPE_INLINE_PIXBUF:
                      proc_def->icon_data_length = icon_data_length;
                      proc_def->icon_data        = g_memdup (icon_data,
                                                             icon_data_length);
                      break;
                    }

                  break;
                }
            }

          if (! list)
            success = FALSE;
        }
      else
        success = FALSE;
    }

  return procedural_db_return_args (&plugin_icon_register_proc, success);
}

static ProcArg plugin_icon_register_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "procedure_name",
    "The procedure for which to install the icon"
  },
  {
    GIMP_PDB_INT32,
    "icon_type",
    "The type of the icon"
  },
  {
    GIMP_PDB_INT32,
    "icon_data_length",
    "The length of 'icon_data': 0 < icon_data_length"
  },
  {
    GIMP_PDB_INT8ARRAY,
    "icon_data",
    "The procedure's icon. The format depends on the 'icon_type' parameter"
  }
};

static ProcRecord plugin_icon_register_proc =
{
  "gimp_plugin_icon_register",
  "Register an icon for a plug-in procedure.",
  "This procedure installs an icon for the given procedure.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer <mitch@gimp.org>",
  "2004",
  GIMP_INTERNAL,
  4,
  plugin_icon_register_inargs,
  0,
  NULL,
  { { plugin_icon_register_invoker } }
};
