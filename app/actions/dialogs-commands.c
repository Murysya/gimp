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

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "actions-types.h"

#include "core/gimp.h"
#include "core/gimpcontext.h"

#include "widgets/gimpdialogfactory.h"
#include "widgets/gimpdockable.h"
#include "widgets/gimpdockbook.h"
#include "widgets/gimpimagedock.h"

#include "display/gimpdisplay.h"

#include "gui/dialogs.h"
#include "dialogs-commands.h"


#define return_if_no_widget(widget,data) \
  if (GIMP_IS_DISPLAY (data)) \
    widget = ((GimpDisplay *) data)->shell; \
  else if (GIMP_IS_GIMP (data)) \
    widget = dialogs_get_toolbox (); \
  else if (GIMP_IS_DOCK (data)) \
    widget = data; \
  else \
    widget = NULL; \
  \
  if (! widget) \
    return


/*  local function prototypes  */

static void   dialogs_create_dock (GdkScreen   *screen,
                                   gboolean     show_image_menu,
                                   const gchar *tabs[],
                                   gint         n_tabs);


/*  public functions  */

void
dialogs_show_toolbox_cmd_callback (GtkAction *action,
                                   gpointer   data)
{
  dialogs_show_toolbox ();
}

void
dialogs_create_toplevel_cmd_callback (GtkAction   *action,
                                      const gchar *value,
				      gpointer     data)
{
  GtkWidget *widget;
  return_if_no_widget (widget, data);

  if (value)
    gimp_dialog_factory_dialog_new (global_dialog_factory,
                                    gtk_widget_get_screen (widget),
                                    value, -1);
}

void
dialogs_create_dockable_cmd_callback (GtkAction   *action,
                                      const gchar *value,
				      gpointer     data)
{
  GtkWidget *widget;
  return_if_no_widget (widget, data);

  if (value)
    gimp_dialog_factory_dialog_raise (global_dock_factory,
                                      gtk_widget_get_screen (widget),
                                      value, -1);
}

void
dialogs_create_lc_cmd_callback (GtkAction *action,
                                gpointer   data)
{
  GtkWidget *widget;
  return_if_no_widget (widget, data);

  static const gchar *tabs[] =
  {
    "gimp-layer-list",
    "gimp-channel-list",
    "gimp-vectors-list",
    "gimp-undo-history"
  };

  dialogs_create_dock (gtk_widget_get_screen (widget), TRUE,
                       tabs, G_N_ELEMENTS (tabs));
}

void
dialogs_create_data_cmd_callback (GtkAction *action,
                                  gpointer   data)
{
  GtkWidget *widget;
  return_if_no_widget (widget, data);

  static const gchar *tabs[] =
  {
    "gimp-brush-grid",
    "gimp-pattern-grid",
    "gimp-gradient-list",
    "gimp-palette-list",
    "gimp-font-list"
  };

  dialogs_create_dock (gtk_widget_get_screen (widget), FALSE,
                       tabs, G_N_ELEMENTS (tabs));
}

void
dialogs_create_stuff_cmd_callback (GtkAction *action,
                                   gpointer   data)
{
  GtkWidget *widget;
  return_if_no_widget (widget, data);

  static const gchar *tabs[] =
  {
    "gimp-buffer-list",
    "gimp-image-list",
    "gimp-document-list",
    "gimp-template-list"
  };

  dialogs_create_dock (gtk_widget_get_screen (widget), FALSE,
                       tabs, G_N_ELEMENTS (tabs));
}

void
dialogs_show_toolbox (void)
{
  if (! global_toolbox_factory->open_dialogs)
    {
      GtkWidget *toolbox;

      toolbox = gimp_dialog_factory_dock_new (global_toolbox_factory,
                                              gdk_screen_get_default ());

      gtk_widget_show (toolbox);
    }
  else
    {
      GList *list;

      for (list = global_toolbox_factory->open_dialogs;
           list;
           list = g_list_next (list))
        {
          if (GTK_WIDGET_TOPLEVEL (list->data))
            {
              gtk_window_present (GTK_WINDOW (list->data));
              break;
            }
        }
    }
}


/*  private functions  */

static void
dialogs_create_dock (GdkScreen   *screen,
                     gboolean     show_image_menu,
                     const gchar *tabs[],
                     gint         n_tabs)
{
  GtkWidget *dock;
  GtkWidget *dockbook;
  GtkWidget *dockable;
  gint       i;

  dock = gimp_dialog_factory_dock_new (global_dock_factory, screen);

  gimp_image_dock_set_show_image_menu (GIMP_IMAGE_DOCK (dock), show_image_menu);

  dockbook = gimp_dockbook_new (global_dock_factory->menu_factory);

  gimp_dock_add_book (GIMP_DOCK (dock), GIMP_DOCKBOOK (dockbook), 0);

  for (i = 0; i < n_tabs; i++)
    {
      dockable = gimp_dialog_factory_dialog_new (global_dock_factory,
                                                 screen,
                                                 tabs[i], -1);

      if (dockable && ! GIMP_DOCKABLE (dockable)->dockbook)
        gimp_dock_add (GIMP_DOCK (dock), GIMP_DOCKABLE (dockable), -1, -1);
    }

  gtk_widget_show (dock);
}
