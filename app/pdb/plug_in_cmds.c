/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2000 Spencer Kimball and Peter Mattis
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

#include <gtk/gtk.h>

#include "core/core-types.h"
#include "procedural_db.h"

#include "appenv.h"
#include "plug_in.h"

#ifdef HAVE_GLIBC_REGEX
#include <regex.h>
#else
#include "regexrepl.h"
#endif


static ProcRecord progress_init_proc;
static ProcRecord progress_update_proc;
static ProcRecord temp_PDB_name_proc;
static ProcRecord plugins_query_proc;
static ProcRecord plugin_domain_register_proc;
static ProcRecord plugin_help_register_proc;

void
register_plug_in_procs (void)
{
  procedural_db_register (&progress_init_proc);
  procedural_db_register (&progress_update_proc);
  procedural_db_register (&temp_PDB_name_proc);
  procedural_db_register (&plugins_query_proc);
  procedural_db_register (&plugin_domain_register_proc);
  procedural_db_register (&plugin_help_register_proc);
}

static int
match_strings (regex_t *preg,
               gchar   *a)
{
  return regexec (preg, a, 0, NULL, 0);
}

static Argument *
progress_init_invoker (Argument *args)
{
  gboolean success = FALSE;
  gchar *message;
  gint32 gdisplay;

  message = (gchar *) args[0].value.pdb_pointer;

  gdisplay = args[1].value.pdb_int;

  if (current_plug_in && current_plug_in->open)
    {
      success = TRUE;
      if (!no_interface)
	plug_in_progress_init (current_plug_in, message, gdisplay);
    }

  return procedural_db_return_args (&progress_init_proc, success);
}

static ProcArg progress_init_inargs[] =
{
  {
    PDB_STRING,
    "message",
    "Message to use in the progress dialog"
  },
  {
    PDB_INT32,
    "gdisplay",
    "GDisplay to update progressbar in, or -1 for a seperate window"
  }
};

static ProcRecord progress_init_proc =
{
  "gimp_progress_init",
  "Initializes the progress bar for the current plug-in.",
  "Initializes the progress bar for the current plug-in. It is only valid to call this procedure from a plug-in.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  2,
  progress_init_inargs,
  0,
  NULL,
  { { progress_init_invoker } }
};

static Argument *
progress_update_invoker (Argument *args)
{
  gboolean success = FALSE;
  gdouble percentage;

  percentage = args[0].value.pdb_float;

  if (current_plug_in && current_plug_in->open)
    {
      success = TRUE;
      if (!no_interface)
	plug_in_progress_update (current_plug_in, percentage);
    }

  return procedural_db_return_args (&progress_update_proc, success);
}

static ProcArg progress_update_inargs[] =
{
  {
    PDB_FLOAT,
    "percentage",
    "Percentage of progress completed"
  }
};

static ProcRecord progress_update_proc =
{
  "gimp_progress_update",
  "Updates the progress bar for the current plug-in.",
  "Updates the progress bar for the current plug-in. It is only valid to call this procedure from a plug-in.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  1,
  progress_update_inargs,
  0,
  NULL,
  { { progress_update_invoker } }
};

static Argument *
temp_PDB_name_invoker (Argument *args)
{
  Argument *return_args;
  gchar *temp_name;
  static gint proc_number = 0;

  temp_name = g_strdup_printf ("temp_plugin_number_%d", proc_number++);

  return_args = procedural_db_return_args (&temp_PDB_name_proc, TRUE);
  return_args[1].value.pdb_pointer = temp_name;

  return return_args;
}

static ProcArg temp_PDB_name_outargs[] =
{
  {
    PDB_STRING,
    "temp_name",
    "A unique temporary name for a temporary PDB entry"
  }
};

static ProcRecord temp_PDB_name_proc =
{
  "gimp_temp_PDB_name",
  "Generates a unique temporary PDB name.",
  "This procedure generates a temporary PDB entry name that is guaranteed to be unique. It is many used by the interactive popup dialogs to generate a PDB entry name.",
  "Andy Thomas",
  "Andy Thomas",
  "1998",
  PDB_INTERNAL,
  0,
  NULL,
  1,
  temp_PDB_name_outargs,
  { { temp_PDB_name_invoker } }
};

static Argument *
plugins_query_invoker (Argument *args)
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
  PlugInProcDef *proc_def;
  GSList *tmp = NULL;
  gint i = 0;
  regex_t sregex;

  search_str = (gchar *) args[0].value.pdb_pointer;

  if (search_str && strlen (search_str))
    regcomp (&sregex, search_str, REG_ICASE);
  else
    search_str = NULL;

  /* count number of plugin entries, then allocate 4 arrays of correct size
   * where we can store the strings.
   */

  tmp = proc_defs;
  while (tmp)
    {
      proc_def = tmp->data;
      tmp = tmp->next;

      if (proc_def->prog && proc_def->menu_path)
	{
	  gchar *name = strrchr (proc_def->menu_path, '/');

	  if (name)
	    name = name + 1;
	  else
	    name = proc_def->menu_path;

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

  tmp = proc_defs;
  while (tmp)
    {
      if (i > num_plugins)
	g_error ("Internal error counting plugins");

      proc_def = tmp->data;
      tmp = tmp->next;

      if (proc_def->prog && proc_def->menu_path)
	{
	  ProcRecord *pr = &proc_def->db_info;

	  gchar *name = strrchr (proc_def->menu_path, '/');

	  if (name)
	    name = name + 1;
	  else
	    name = proc_def->menu_path;

	  if (search_str && match_strings(&sregex,name))
	    continue;

	  menu_strs[i]     = g_strdup (proc_def->menu_path);
	  accel_strs[i]    = g_strdup (proc_def->accelerator);
	  prog_strs[i]     = g_strdup (proc_def->prog);
	  types_strs[i]    = g_strdup (proc_def->image_types);
	  realname_strs[i] = g_strdup (pr->name);
	  time_ints[i]     = proc_def->mtime;

	  i++;
	}
    }

  /* This I hope frees up internal stuff */
  if (search_str)
    free (sregex.buffer);

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
    PDB_STRING,
    "search_string",
    "If not an empty string then use this as a search pattern"
  }
};

static ProcArg plugins_query_outargs[] =
{
  {
    PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    PDB_STRINGARRAY,
    "menu_path",
    "The menu path of the plugin"
  },
  {
    PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    PDB_STRINGARRAY,
    "plugin_accelerator",
    "String representing keyboard accelerator (could be empty string)"
  },
  {
    PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    PDB_STRINGARRAY,
    "plugin_location",
    "Location of the plugin program"
  },
  {
    PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    PDB_STRINGARRAY,
    "plugin_image_type",
    "Type of image that this plugin will work on"
  },
  {
    PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    PDB_INT32ARRAY,
    "plugin_install_time",
    "Time that the plugin was installed"
  },
  {
    PDB_INT32,
    "num_plugins",
    "The number of plugins"
  },
  {
    PDB_STRINGARRAY,
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
  PDB_INTERNAL,
  1,
  plugins_query_inargs,
  12,
  plugins_query_outargs,
  { { plugins_query_invoker } }
};

static Argument *
plugin_domain_register_invoker (Argument *args)
{
  gboolean success = TRUE;
  gchar *domain_name;
  gchar *domain_path;
  PlugInDef *plug_in_def;

  domain_name = (gchar *) args[0].value.pdb_pointer;
  if (domain_name == NULL)
    success = FALSE;

  domain_path = (gchar *) args[1].value.pdb_pointer;

  if (success)
    {
      if (current_plug_in && current_plug_in->query)
	{
	  plug_in_def = current_plug_in->user_data;
    
	  if (plug_in_def->locale_domain)
	    g_free (plug_in_def->locale_domain);      
	  plug_in_def->locale_domain = g_strdup (domain_name);
    
	  if (plug_in_def->locale_path);
	    g_free (plug_in_def->locale_path);
	  plug_in_def->locale_path = domain_path ? g_strdup (domain_path) : NULL;
	}
    }

  return procedural_db_return_args (&plugin_domain_register_proc, success);
}

static ProcArg plugin_domain_register_inargs[] =
{
  {
    PDB_STRING,
    "domain_name",
    "The name of the textdomain (must be unique)."
  },
  {
    PDB_STRING,
    "domain_path",
    "The absolute path to the compiled message catalog (may be NULL)."
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
  PDB_INTERNAL,
  2,
  plugin_domain_register_inargs,
  0,
  NULL,
  { { plugin_domain_register_invoker } }
};

static Argument *
plugin_help_register_invoker (Argument *args)
{
  gboolean success = TRUE;
  gchar *help_path;
  PlugInDef *plug_in_def;

  help_path = (gchar *) args[0].value.pdb_pointer;
  if (help_path == NULL)
    success = FALSE;

  if (success)
    {
      if (current_plug_in && current_plug_in->query)
	{
	  plug_in_def = current_plug_in->user_data;
    
	  if (plug_in_def->help_path)
	    g_free (plug_in_def->help_path);
	  plug_in_def->help_path = g_strdup (help_path);
	}
    }

  return procedural_db_return_args (&plugin_help_register_proc, success);
}

static ProcArg plugin_help_register_inargs[] =
{
  {
    PDB_STRING,
    "help_path",
    "The rootdir of the plug-in's help pages"
  }
};

static ProcRecord plugin_help_register_proc =
{
  "gimp_plugin_help_register",
  "Register a help path for a plug-in.",
  "This procedure changes the help rootdir for the plug-in which calls it. All subsequent calls of gimp_help from this plug-in will be interpreted relative to this rootdir. This procedure can only be called in the query function of a plug-in and it has to be called before any procedure is installed.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer <mitch@gimp.org>",
  "2000",
  PDB_INTERNAL,
  1,
  plugin_help_register_inargs,
  0,
  NULL,
  { { plugin_help_register_invoker } }
};
