/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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

#include <string.h>

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpbase/gimpprotocol.h"
#include "libgimpcolor/gimpcolor.h"

#include "plug-in-types.h"

#include "core/gimpparamspecs.h"

#include "pdb/gimpargument.h"
#include "pdb/procedural_db.h"

#include "plug-in.h"
#include "plug-in-params.h"


GimpArgument *
plug_in_params_to_args (GimpArgumentSpec *proc_args,
                        gint              n_proc_args,
                        GPParam          *params,
			gint              n_params,
			gboolean          full_copy)
{
  GimpArgument *args;
  gint          count;
  gint          i;

  g_return_val_if_fail ((proc_args != NULL && n_proc_args  > 0) ||
                        (proc_args == NULL && n_proc_args == 0), NULL);
  g_return_val_if_fail ((params != NULL && n_params  > 0) ||
                        (params == NULL && n_params == 0), NULL);

  if (! params)
    return NULL;

  args = g_new0 (GimpArgument, n_params);

  for (i = 0; i < n_params; i++)
    {
      GValue *value = &args[i].value;

      if (i < n_proc_args && proc_args[i].type == params[i].type)
        {
          gimp_argument_init (&args[i], &proc_args[i]);
        }
      else
        {
          gimp_argument_init_compat (&args[i], params[i].type);
        }

      switch (args[i].type)
	{
	case GIMP_PDB_INT32:
          if (G_VALUE_HOLDS_INT (value))
            g_value_set_int (value, params[i].data.d_int32);
          else if (G_VALUE_HOLDS_ENUM (value))
            g_value_set_enum (value, params[i].data.d_int32);
          else if (G_VALUE_HOLDS_BOOLEAN (value))
            g_value_set_boolean (value, params[i].data.d_int32 ? TRUE : FALSE);
          else
            g_return_val_if_reached (args);
	  break;

	case GIMP_PDB_INT16:
	  g_value_set_int (value, params[i].data.d_int16);
	  break;

	case GIMP_PDB_INT8:
	  g_value_set_uint (value, params[i].data.d_int8);
	  break;

	case GIMP_PDB_FLOAT:
	  g_value_set_double (value, params[i].data.d_float);
	  break;

	case GIMP_PDB_STRING:
	  if (full_copy)
	    g_value_set_string (value, params[i].data.d_string);
	  else
	    g_value_set_static_string (value, params[i].data.d_string);
	  break;

	case GIMP_PDB_INT32ARRAY:
	  if (full_copy)
	    {
	      count = g_value_get_int (&args[i - 1].value);
	      g_value_set_pointer (value,
                                   g_memdup (params[i].data.d_int32array,
                                             count * sizeof (gint32)));
	    }
	  else
	    {
	      g_value_set_pointer (value, params[i].data.d_int32array);
	    }
	  break;

	case GIMP_PDB_INT16ARRAY:
	  if (full_copy)
	    {
	      count = g_value_get_int (&args[i - 1].value);
	      g_value_set_pointer (value,
                                   g_memdup (params[i].data.d_int16array,
                                             count * sizeof (gint16)));
	    }
	  else
	    {
	      g_value_set_pointer (value, params[i].data.d_int16array);
	    }
	  break;

	case GIMP_PDB_INT8ARRAY:
	  if (full_copy)
	    {
	      count = g_value_get_int (&args[i - 1].value);
	      g_value_set_pointer (value,
                                   g_memdup (params[i].data.d_int8array,
                                             count));
	    }
	  else
	    {
	      g_value_set_pointer (value, params[i].data.d_int8array);
	    }
	  break;

	case GIMP_PDB_FLOATARRAY:
	  if (full_copy)
	    {
	      count = g_value_get_int (&args[i - 1].value);
              g_value_set_pointer (value,
                                   g_memdup (params[i].data.d_floatarray,
                                             count * sizeof (gdouble)));
	    }
	  else
	    {
	      g_value_set_pointer (value, params[i].data.d_floatarray);
	    }
	  break;

	case GIMP_PDB_STRINGARRAY:
	  if (full_copy)
	    {
              gchar **array;
              gint    j;

	      count = g_value_get_int (&args[i - 1].value);

	      array = g_new (gchar *, count);
              g_value_set_pointer (value, array);

	      for (j = 0; j < count; j++)
		array[j] = g_strdup (params[i].data.d_stringarray[j]);
	    }
	  else
	    {
	      g_value_set_pointer (value, params[i].data.d_stringarray);
	    }
	  break;

	case GIMP_PDB_COLOR:
	  gimp_value_set_rgb (value, &params[i].data.d_color);
	  break;

	case GIMP_PDB_REGION:
	  g_message ("the \"region\" argument type is not supported");
	  break;

	case GIMP_PDB_DISPLAY:
	  g_value_set_int (value, params[i].data.d_display);
	  break;

	case GIMP_PDB_IMAGE:
	  g_value_set_int (value, params[i].data.d_image);
	  break;

	case GIMP_PDB_LAYER:
	  g_value_set_int (value, params[i].data.d_layer);
	  break;

	case GIMP_PDB_CHANNEL:
	  g_value_set_int (value, params[i].data.d_channel);
	  break;

	case GIMP_PDB_DRAWABLE:
	  g_value_set_int (value, params[i].data.d_drawable);
	  break;

	case GIMP_PDB_SELECTION:
	  g_value_set_int (value, params[i].data.d_selection);
	  break;

	case GIMP_PDB_BOUNDARY:
	  g_message ("the \"boundary\" arg type is not currently supported");
	  break;

	case GIMP_PDB_VECTORS:
	  g_value_set_int (value, params[i].data.d_vectors);
	  break;

	case GIMP_PDB_PARASITE:
	  if (full_copy)
	    g_value_set_boxed (value, &params[i].data.d_parasite);
	  else
	    g_value_set_static_boxed (value, &params[i].data.d_parasite);
	  break;

	case GIMP_PDB_STATUS:
	  g_value_set_enum (value, params[i].data.d_status);
	  break;

	case GIMP_PDB_END:
	  break;
	}
    }

  return args;
}

GPParam *
plug_in_args_to_params (GimpArgument *args,
			gint          n_args,
			gboolean      full_copy)
{
  GPParam *params;
  gint     count;
  gint     i;

  g_return_val_if_fail ((args != NULL && n_args  > 0) ||
                        (args == NULL && n_args == 0), NULL);

  if (! args)
    return NULL;

  params = g_new0 (GPParam, n_args);

  for (i = 0; i < n_args; i++)
    {
      GValue *value = &args[i].value;

      params[i].type = args[i].type;

      switch (args[i].type)
	{
	case GIMP_PDB_INT32:
          if (G_VALUE_HOLDS_INT (value))
            params[i].data.d_int32 = g_value_get_int (value);
          else if (G_VALUE_HOLDS_ENUM (value))
            params[i].data.d_int32 = g_value_get_enum (value);
          else if (G_VALUE_HOLDS_BOOLEAN (value))
            params[i].data.d_int32 = g_value_get_boolean (value);
          else
            {
              g_printerr ("unhandled GIMP_PDB_INT32 type: %s\n",
                          g_type_name (value->g_type));
              g_return_val_if_reached (params);
            }
	  break;

	case GIMP_PDB_INT16:
	  params[i].data.d_int16 = g_value_get_int (value);
	  break;

	case GIMP_PDB_INT8:
	  params[i].data.d_int8 = g_value_get_int (value);
	  break;

	case GIMP_PDB_FLOAT:
	  params[i].data.d_float = g_value_get_double (value);
	  break;

	case GIMP_PDB_STRING:
	  if (full_copy)
	    params[i].data.d_string = g_value_dup_string (value);
	  else
	    params[i].data.d_string = (gchar *) g_value_get_string (value);
	  break;

	case GIMP_PDB_INT32ARRAY:
	  if (full_copy)
	    {
              count = g_value_get_int (&args[i - 1].value);
	      params[i].data.d_int32array =
                g_memdup (g_value_get_pointer (value),
                          count * sizeof (gint32));
	    }
	  else
	    {
	      params[i].data.d_int32array = g_value_get_pointer (value);
	    }
	  break;

	case GIMP_PDB_INT16ARRAY:
	  if (full_copy)
	    {
              count = g_value_get_int (&args[i - 1].value);
	      params[i].data.d_int16array =
                g_memdup (g_value_get_pointer (value),
                          count * sizeof (gint16));
	    }
	  else
	    {
	      params[i].data.d_int16array = g_value_get_pointer (value);
	    }
	  break;

	case GIMP_PDB_INT8ARRAY:
	  if (full_copy)
	    {
              count = g_value_get_int (&args[i - 1].value);
	      params[i].data.d_int8array =
                g_memdup (g_value_get_pointer (value), count);
	    }
	  else
	    {
	      params[i].data.d_int8array = g_value_get_pointer (value);
	    }
	  break;

	case GIMP_PDB_FLOATARRAY:
	  if (full_copy)
	    {
              count = g_value_get_int (&args[i - 1].value);
	      params[i].data.d_floatarray =
                g_memdup (g_value_get_pointer (value),
                          count * sizeof (gdouble));
	    }
	  else
	    {
	      params[i].data.d_floatarray = g_value_get_pointer (value);
	    }
	  break;

	case GIMP_PDB_STRINGARRAY:
	  if (full_copy)
	    {
              gchar **array;
              gint    j;

              count = g_value_get_int (&args[i - 1].value);

	      array = g_value_get_pointer (value);
	      params[i].data.d_stringarray = g_new (gchar *, count);

	      for (j = 0; j < count; j++)
		params[i].data.d_stringarray[j] = g_strdup (array[j]);
	    }
	  else
	    {
	      params[i].data.d_stringarray = g_value_get_pointer (value);
	    }
	  break;

	case GIMP_PDB_COLOR:
	  gimp_value_get_rgb (value, &params[i].data.d_color);
	  break;

	case GIMP_PDB_REGION:
	  g_message ("the \"region\" argument type is not supported");
	  break;

	case GIMP_PDB_DISPLAY:
	  params[i].data.d_display = g_value_get_int (value);
	  break;

	case GIMP_PDB_IMAGE:
	  params[i].data.d_image = g_value_get_int (value);
	  break;

	case GIMP_PDB_LAYER:
	  params[i].data.d_layer = g_value_get_int (value);
	  break;

	case GIMP_PDB_CHANNEL:
	  params[i].data.d_channel = g_value_get_int (value);
	  break;

	case GIMP_PDB_DRAWABLE:
	  params[i].data.d_drawable = g_value_get_int (value);
	  break;

	case GIMP_PDB_SELECTION:
	  params[i].data.d_selection = g_value_get_int (value);
	  break;

	case GIMP_PDB_BOUNDARY:
	  g_message ("the \"boundary\" arg type is not currently supported");
	  break;

	case GIMP_PDB_VECTORS:
	  params[i].data.d_vectors = g_value_get_int (value);
	  break;

	case GIMP_PDB_PARASITE:
          {
            GimpParasite *parasite = (full_copy ?
                                      g_value_dup_boxed (value) :
                                      g_value_get_boxed (value));

            if (parasite)
              {
                params[i].data.d_parasite.name  = parasite->name;
                params[i].data.d_parasite.flags = parasite->flags;
                params[i].data.d_parasite.size  = parasite->size;
                params[i].data.d_parasite.data  = parasite->data;

                if (full_copy)
                  g_free (parasite);
              }
            else
              {
                params[i].data.d_parasite.name  = NULL;
                params[i].data.d_parasite.flags = 0;
                params[i].data.d_parasite.size  = 0;
                params[i].data.d_parasite.data  = NULL;
              }
          }
	  break;

	case GIMP_PDB_STATUS:
	  params[i].data.d_status = g_value_get_enum (value);
	  break;

	case GIMP_PDB_END:
	  break;
	}
    }

  return params;
}

void
plug_in_params_destroy (GPParam  *params,
			gint      n_params,
			gboolean  full_destroy)
{
  gint i, j;

  if (full_destroy)
    {
      for (i = 0; i < n_params; i++)
        {
          switch (params[i].type)
            {
            case GIMP_PDB_INT32:
            case GIMP_PDB_INT16:
            case GIMP_PDB_INT8:
            case GIMP_PDB_FLOAT:
              break;

            case GIMP_PDB_STRING:
              g_free (params[i].data.d_string);
              break;

            case GIMP_PDB_INT32ARRAY:
              g_free (params[i].data.d_int32array);
              break;

            case GIMP_PDB_INT16ARRAY:
              g_free (params[i].data.d_int16array);
              break;

            case GIMP_PDB_INT8ARRAY:
              g_free (params[i].data.d_int8array);
              break;

            case GIMP_PDB_FLOATARRAY:
              g_free (params[i].data.d_floatarray);
              break;

            case GIMP_PDB_STRINGARRAY:
              for (j = 0; j < params[i-1].data.d_int32; j++)
                g_free (params[i].data.d_stringarray[j]);
              g_free (params[i].data.d_stringarray);
              break;

            case GIMP_PDB_COLOR:
              break;

            case GIMP_PDB_REGION:
              g_message ("the \"region\" argument type is not supported");
              break;

            case GIMP_PDB_DISPLAY:
            case GIMP_PDB_IMAGE:
            case GIMP_PDB_LAYER:
            case GIMP_PDB_CHANNEL:
            case GIMP_PDB_DRAWABLE:
            case GIMP_PDB_SELECTION:
            case GIMP_PDB_BOUNDARY:
              g_message ("the \"boundary\" arg type is not currently supported");
              break;

            case GIMP_PDB_VECTORS:
              break;

            case GIMP_PDB_PARASITE:
              if (params[i].data.d_parasite.data)
                {
                  g_free (params[i].data.d_parasite.name);
                  g_free (params[i].data.d_parasite.data);
                  params[i].data.d_parasite.name = NULL;
                  params[i].data.d_parasite.data = NULL;
                }
              break;

            case GIMP_PDB_STATUS:
            case GIMP_PDB_END:
              break;
            }
        }
    }

  g_free (params);
}

gboolean
plug_in_param_defs_check (const gchar *plug_in_name,
                          const gchar *plug_in_prog,
                          const gchar *procedure_name,
                          const gchar *menu_path,
                          GPParamDef  *params,
                          guint32      n_args,
                          GPParamDef  *return_vals,
                          guint32      n_return_vals,
                          GError     **error)
{
  GimpArgumentSpec *args;
  GimpArgumentSpec *return_args;
  gboolean          success;
  gint              i;

  args = g_new0 (GimpArgumentSpec, n_args);
  for (i = 0; i < n_args; i++)
    args[i].type = params[i].type;

  return_args = g_new0 (GimpArgumentSpec, n_return_vals);
  for (i = 0; i < n_return_vals; i++)
    return_args[i].type = return_vals[i].type;

  success = plug_in_proc_args_check (plug_in_name,
                                     plug_in_prog,
                                     procedure_name,
                                     menu_path,
                                     args,
                                     n_args,
                                     return_args,
                                     n_return_vals,
                                     error);

  g_free (args);
  g_free (return_args);

  return success;
}

gboolean
plug_in_proc_args_check (const gchar       *plug_in_name,
                         const gchar       *plug_in_prog,
                         const gchar       *procedure_name,
                         const gchar       *menu_path,
                         GimpArgumentSpec  *args,
                         guint32            n_args,
                         GimpArgumentSpec  *return_vals,
                         guint32            n_return_vals,
                         GError           **error)
{
  gchar *prefix;
  gchar *p;

  g_return_val_if_fail (plug_in_name != NULL, FALSE);
  g_return_val_if_fail (plug_in_prog != NULL, FALSE);
  g_return_val_if_fail (procedure_name != NULL, FALSE);
  g_return_val_if_fail (menu_path != NULL, FALSE);
  g_return_val_if_fail (args == NULL || n_args > 0, FALSE);
  g_return_val_if_fail (return_vals == NULL || n_return_vals > 0, FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  prefix = g_strdup (menu_path);

  p = strchr (prefix, '>') + 1;
  if (p)
    *p = '\0';

  if (strcmp (prefix, "<Toolbox>") == 0 ||
      strcmp (prefix, "<Image>")   == 0)
    {
      if ((n_args < 1) ||
          (args[0].type != GIMP_PDB_INT32))
        {
          g_set_error (error, 0, 0,
                       "Plug-In \"%s\"\n(%s)\n\n"
                       "attempted to install %s procedure \"%s\" "
                       "which does not take the standard %s Plug-In "
                       "arguments.\n"
                       "(INT32)",
                       gimp_filename_to_utf8 (plug_in_name),
                       gimp_filename_to_utf8 (plug_in_prog),
                       prefix, procedure_name, prefix);
          goto failure;
        }
    }
  else if (strcmp (prefix, "<Load>") == 0)
    {
      if ((n_args < 3)                      ||
          (args[0].type != GIMP_PDB_INT32)  ||
          (args[1].type != GIMP_PDB_STRING) ||
          (args[2].type != GIMP_PDB_STRING))
        {
          g_set_error (error, 0, 0,
                       "Plug-In \"%s\"\n(%s)\n\n"
                       "attempted to install <Load> procedure \"%s\" "
                       "which does not take the standard <Load> Plug-In "
                       "arguments.\n"
                       "(INT32, STRING, STRING)",
                       gimp_filename_to_utf8 (plug_in_name),
                       gimp_filename_to_utf8 (plug_in_prog),
                       procedure_name);
          goto failure;
        }
    }
  else if (strcmp (prefix, "<Save>") == 0)
    {
      if ((n_args < 5)                        ||
          (args[0].type != GIMP_PDB_INT32)    ||
          (args[1].type != GIMP_PDB_IMAGE)    ||
          (args[2].type != GIMP_PDB_DRAWABLE) ||
          (args[3].type != GIMP_PDB_STRING)   ||
          (args[4].type != GIMP_PDB_STRING))
        {
          g_set_error (error, 0, 0,
                       "Plug-In \"%s\"\n(%s)\n\n"
                       "attempted to install <Save> procedure \"%s\" "
                       "which does not take the standard <Save> Plug-In "
                       "arguments.\n"
                       "(INT32, IMAGE, DRAWABLE, STRING, STRING)",
                       gimp_filename_to_utf8 (plug_in_name),
                       gimp_filename_to_utf8 (plug_in_prog),
                       procedure_name);
          goto failure;
        }
    }
  else if (strcmp (prefix, "<Brushes>")   == 0 ||
           strcmp (prefix, "<Gradients>") == 0 ||
           strcmp (prefix, "<Palettes>")  == 0 ||
           strcmp (prefix, "<Patterns>")  == 0 ||
           strcmp (prefix, "<Fonts>")     == 0 ||
           strcmp (prefix, "<Buffers>")   == 0)
    {
      if ((n_args < 1) ||
          (args[0].type != GIMP_PDB_INT32))
        {
          g_set_error (error, 0, 0,
                       "Plug-In \"%s\"\n(%s)\n\n"
                       "attempted to install %s procedure \"%s\" "
                       "which does not take the standard %s Plug-In "
                       "arguments.\n"
                       "(INT32)",
                       gimp_filename_to_utf8 (plug_in_name),
                       gimp_filename_to_utf8 (plug_in_prog),
                       prefix, procedure_name, prefix);
          goto failure;
        }
    }
  else
    {
      g_set_error (error, 0, 0,
                   "Plug-In \"%s\"\n(%s)\n"
                   "attempted to install procedure \"%s\" "
                   "in the invalid menu location \"%s\".\n"
                   "Use either \"<Toolbox>\", \"<Image>\", "
                   "\"<Load>\", \"<Save>\", \"<Brushes>\", "
                   "\"<Gradients>\", \"<Palettes>\", \"<Patterns>\" or "
                   "\"<Buffers>\".",
                   gimp_filename_to_utf8 (plug_in_name),
                   gimp_filename_to_utf8 (plug_in_prog),
                   procedure_name,
                   menu_path);
      goto failure;
    }

  p = strchr (menu_path, '>') + 1;

  if (*p != '/' && *p != '\0')
    {
      g_set_error (error, 0, 0,
                   "Plug-In \"%s\"\n(%s)\n"
                   "attempted to install procedure \"%s\"\n"
                   "in the invalid menu location \"%s\".\n"
                   "The menu path must look like either \"<Prefix>\" "
                   "or \"<Prefix>/path/to/item\".",
                   gimp_filename_to_utf8 (plug_in_name),
                   gimp_filename_to_utf8 (plug_in_prog),
                   procedure_name,
                   menu_path);
      goto failure;
    }

  g_free (prefix);

  return TRUE;

 failure:
  g_free (prefix);

  return FALSE;
}
