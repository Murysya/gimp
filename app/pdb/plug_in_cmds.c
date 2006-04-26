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

#include "pdb-types.h"
#include "gimppdb.h"
#include "gimpprocedure.h"
#include "core/gimpparamspecs.h"

#include "core/gimp.h"
#include "gimppluginprocedure.h"
#include "plug-in/plug-in-def.h"
#include "plug-in/plug-in-menu-branch.h"
#include "plug-in/plug-in.h"
#include "plug-in/plug-ins-query.h"
#include "plug-in/plug-ins.h"


static GValueArray *
plugins_query_invoker (GimpProcedure     *procedure,
                       Gimp              *gimp,
                       GimpContext       *context,
                       GimpProgress      *progress,
                       const GValueArray *args)
{
  GValueArray *return_vals;
  const gchar *search_string;
  gint32 num_plugins = 0;
  gchar **menu_path = NULL;
  gchar **plugin_accelerator = NULL;
  gchar **plugin_location = NULL;
  gchar **plugin_image_type = NULL;
  gint32 *plugin_install_time = NULL;
  gchar **plugin_real_name = NULL;

  search_string = g_value_get_string (&args->values[0]);

  num_plugins = plug_ins_query (gimp, search_string,
                                &menu_path,
                                &plugin_accelerator,
                                &plugin_location,
                                &plugin_image_type,
                                &plugin_real_name,
                                &plugin_install_time);

  return_vals = gimp_procedure_get_return_values (procedure, TRUE);

  g_value_set_int (&return_vals->values[1], num_plugins);
  gimp_value_take_stringarray (&return_vals->values[2], menu_path, num_plugins);
  g_value_set_int (&return_vals->values[3], num_plugins);
  gimp_value_take_stringarray (&return_vals->values[4], plugin_accelerator, num_plugins);
  g_value_set_int (&return_vals->values[5], num_plugins);
  gimp_value_take_stringarray (&return_vals->values[6], plugin_location, num_plugins);
  g_value_set_int (&return_vals->values[7], num_plugins);
  gimp_value_take_stringarray (&return_vals->values[8], plugin_image_type, num_plugins);
  g_value_set_int (&return_vals->values[9], num_plugins);
  gimp_value_take_int32array (&return_vals->values[10], plugin_install_time, num_plugins);
  g_value_set_int (&return_vals->values[11], num_plugins);
  gimp_value_take_stringarray (&return_vals->values[12], plugin_real_name, num_plugins);

  return return_vals;
}

static GValueArray *
plugin_domain_register_invoker (GimpProcedure     *procedure,
                                Gimp              *gimp,
                                GimpContext       *context,
                                GimpProgress      *progress,
                                const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *domain_name;
  const gchar *domain_path;

  domain_name = g_value_get_string (&args->values[0]);
  domain_path = g_value_get_string (&args->values[1]);

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

  return gimp_procedure_get_return_values (procedure, success);
}

static GValueArray *
plugin_help_register_invoker (GimpProcedure     *procedure,
                              Gimp              *gimp,
                              GimpContext       *context,
                              GimpProgress      *progress,
                              const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *domain_name;
  const gchar *domain_uri;

  domain_name = g_value_get_string (&args->values[0]);
  domain_uri = g_value_get_string (&args->values[1]);

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

  return gimp_procedure_get_return_values (procedure, success);
}

static GValueArray *
plugin_menu_register_invoker (GimpProcedure     *procedure,
                              Gimp              *gimp,
                              GimpContext       *context,
                              GimpProgress      *progress,
                              const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *procedure_name;
  const gchar *menu_path;

  procedure_name = g_value_get_string (&args->values[0]);
  menu_path = g_value_get_string (&args->values[1]);

  if (success)
    {
      if (gimp->current_plug_in)
        {
          gchar *canonical = gimp_canonicalize_identifier (procedure_name);

          success = plug_in_menu_register (gimp->current_plug_in,
                                           canonical, menu_path);
          g_free (canonical);
        }
      else
        {
          success = FALSE;
        }
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GValueArray *
plugin_menu_branch_register_invoker (GimpProcedure     *procedure,
                                     Gimp              *gimp,
                                     GimpContext       *context,
                                     GimpProgress      *progress,
                                     const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *menu_path;
  const gchar *menu_name;

  menu_path = g_value_get_string (&args->values[0]);
  menu_name = g_value_get_string (&args->values[1]);

  if (success)
    {
      if (gimp->current_plug_in)
        {
          plug_in_menu_branch_add (gimp, gimp->current_plug_in->prog,
                                   menu_path, menu_name);
        }
      else
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GValueArray *
plugin_icon_register_invoker (GimpProcedure     *procedure,
                              Gimp              *gimp,
                              GimpContext       *context,
                              GimpProgress      *progress,
                              const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *procedure_name;
  gint32 icon_type;
  gint32 icon_data_length;
  const guint8 *icon_data;

  procedure_name = g_value_get_string (&args->values[0]);
  icon_type = g_value_get_enum (&args->values[1]);
  icon_data_length = g_value_get_int (&args->values[2]);
  icon_data = gimp_value_get_int8array (&args->values[3]);

  if (success)
    {
      if (gimp->current_plug_in && gimp->current_plug_in->query)
        {
          GimpPlugInProcedure *proc;
          gchar               *canonical;

          canonical = gimp_canonicalize_identifier (procedure_name);

          proc = gimp_plug_in_procedure_find (gimp->current_plug_in->plug_in_def->procedures,
                                              canonical);

          g_free (canonical);

          if (proc)
            gimp_plug_in_procedure_set_icon (proc, icon_type,
                                             icon_data, icon_data_length);
          else
            success = FALSE;
        }
      else
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

void
register_plug_in_procs (GimpPDB *pdb)
{
  GimpProcedure *procedure;

  /*
   * gimp-plugins-query
   */
  procedure = gimp_procedure_new (plugins_query_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-plugins-query");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-plugins-query",
                                     "Queries the plugin database for its contents.",
                                     "This procedure queries the contents of the plugin database.",
                                     "Andy Thomas",
                                     "Andy Thomas",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("search-string",
                                                       "search string",
                                                       "If not an empty string then use this as a search pattern",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-plugins",
                                                          "num plugins",
                                                          "The number of plugins",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("menu-path",
                                                                 "menu path",
                                                                 "The menu path of the plugin",
                                                                 GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-plugins",
                                                          "num plugins",
                                                          "The number of plugins",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("plugin-accelerator",
                                                                 "plugin accelerator",
                                                                 "String representing keyboard accelerator (could be empty string)",
                                                                 GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-plugins",
                                                          "num plugins",
                                                          "The number of plugins",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("plugin-location",
                                                                 "plugin location",
                                                                 "Location of the plugin program",
                                                                 GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-plugins",
                                                          "num plugins",
                                                          "The number of plugins",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("plugin-image-type",
                                                                 "plugin image type",
                                                                 "Type of image that this plugin will work on",
                                                                 GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-plugins",
                                                          "num plugins",
                                                          "The number of plugins",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32_array ("plugin-install-time",
                                                                "plugin install time",
                                                                "Time that the plugin was installed",
                                                                GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-plugins",
                                                          "num plugins",
                                                          "The number of plugins",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("plugin-real-name",
                                                                 "plugin real name",
                                                                 "The internal name of the plugin",
                                                                 GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-plugin-domain-register
   */
  procedure = gimp_procedure_new (plugin_domain_register_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-plugin-domain-register");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-plugin-domain-register",
                                     "Registers a textdomain for localisation.",
                                     "This procedure adds a textdomain to the list of domains Gimp searches for strings when translating its menu entries. There is no need to call this function for plug-ins that have their strings included in the gimp-std-plugins domain as that is used by default. If the compiled message catalog is not in the standard location, you may specify an absolute path to another location. This procedure can only be called in the query function of a plug-in and it has to be called before any procedure is installed.",
                                     "Sven Neumann <sven@gimp.org>",
                                     "Sven Neumann",
                                     "2000",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("domain-name",
                                                       "domain name",
                                                       "The name of the textdomain (must be unique)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("domain-path",
                                                       "domain path",
                                                       "The absolute path to the compiled message catalog (may be NULL)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-plugin-help-register
   */
  procedure = gimp_procedure_new (plugin_help_register_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-plugin-help-register");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-plugin-help-register",
                                     "Register a help path for a plug-in.",
                                     "This procedure changes the help rootdir for the plug-in which calls it. All subsequent calls of gimp_help from this plug-in will be interpreted relative to this rootdir.",
                                     "Michael Natterer <mitch@gimp.org>",
                                     "Michael Natterer",
                                     "2000",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("domain-name",
                                                       "domain name",
                                                       "The XML namespace of the plug-in's help pages",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("domain-uri",
                                                       "domain uri",
                                                       "The root URI of the plug-in's help pages",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-plugin-menu-register
   */
  procedure = gimp_procedure_new (plugin_menu_register_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-plugin-menu-register");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-plugin-menu-register",
                                     "Register an additional menu path for a plug-in procedure.",
                                     "This procedure installs an additional menu entry for the given procedure.",
                                     "Michael Natterer <mitch@gimp.org>",
                                     "Michael Natterer",
                                     "2004",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("procedure-name",
                                                       "procedure name",
                                                       "The procedure for which to install the menu path",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("menu-path",
                                                       "menu path",
                                                       "The procedure's additional menu path",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-plugin-menu-branch-register
   */
  procedure = gimp_procedure_new (plugin_menu_branch_register_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-plugin-menu-branch-register");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-plugin-menu-branch-register",
                                     "Register a sub-menu.",
                                     "This procedure installs an sub-menu which does not belong to any procedure.",
                                     "Michael Natterer <mitch@gimp.org>",
                                     "Michael Natterer",
                                     "2005",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("menu-path",
                                                       "menu path",
                                                       "The sub-menu's menu path",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("menu-name",
                                                       "menu name",
                                                       "The name of the sub-menu",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-plugin-icon-register
   */
  procedure = gimp_procedure_new (plugin_icon_register_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-plugin-icon-register");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-plugin-icon-register",
                                     "Register an icon for a plug-in procedure.",
                                     "This procedure installs an icon for the given procedure.",
                                     "Michael Natterer <mitch@gimp.org>",
                                     "Michael Natterer",
                                     "2004",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("procedure-name",
                                                       "procedure name",
                                                       "The procedure for which to install the icon",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("icon-type",
                                                  "icon type",
                                                  "The type of the icon: { GIMP_ICON_TYPE_STOCK_ID (0), GIMP_ICON_TYPE_INLINE_PIXBUF (1), GIMP_ICON_TYPE_IMAGE_FILE (2) }",
                                                  GIMP_TYPE_ICON_TYPE,
                                                  GIMP_ICON_TYPE_STOCK_ID,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("icon-data-length",
                                                      "icon data length",
                                                      "The length of 'icon-data' (1 <= icon_data_length)",
                                                      1, G_MAXINT32, 1,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int8_array ("icon-data",
                                                           "icon data",
                                                           "The procedure's icon. The format depends on the 'icon_type' parameter",
                                                           GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);
}
