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

#include "gui-types.h"

#include "widgets/gimpcontainerview.h"
#include "widgets/gimpcontainerview-utils.h"
#include "widgets/gimpdialogfactory.h"
#include "widgets/gimpdockable.h"
#include "widgets/gimpdockbook.h"
#include "widgets/gimpimagedock.h"

#include "dialogs.h"
#include "dialogs-commands.h"


void
dialogs_show_toolbox_cmd_callback (GtkWidget *widget,
                                   gpointer   data,
                                   guint      action)
{
  dialogs_show_toolbox ();
}

void
dialogs_create_toplevel_cmd_callback (GtkWidget *widget,
				      gpointer   data,
				      guint      action)
{
  if (action)
    {
      const gchar *identifier;

      identifier = g_quark_to_string ((GQuark) action);

      if (identifier)
	gimp_dialog_factory_dialog_new (global_dialog_factory, identifier, -1);
    }
}

void
dialogs_create_dockable_cmd_callback (GtkWidget *widget,
				      gpointer   data,
				      guint      action)
{
  if (action)
    {
      const gchar *identifier;

      identifier = g_quark_to_string ((GQuark) action);

      if (identifier)
	gimp_dialog_factory_dialog_raise (global_dock_factory, identifier, -1);
    }
}

void
dialogs_add_tab_cmd_callback (GtkWidget *widget,
			      gpointer   data,
			      guint      action)
{
  GimpDockbook *dockbook;

  dockbook = (GimpDockbook *) gtk_item_factory_popup_data_from_widget (widget);

  if (dockbook && action)
    {
      GtkWidget   *dockable;
      const gchar *identifier;

      identifier = g_quark_to_string ((GQuark) action);

      if (identifier)
	{
	  dockable =
            gimp_dialog_factory_dockable_new (dockbook->dock->dialog_factory,
                                              dockbook->dock,
                                              identifier,
                                              -1);

	  /*  Maybe gimp_dialog_factory_dockable_new() returned an already
	   *  existing singleton dockable, so check if it already is
	   *  attached to a dockbook.
	   */
	  if (dockable && ! GIMP_DOCKABLE (dockable)->dockbook)
	    gimp_dockbook_add (dockbook, GIMP_DOCKABLE (dockable), -1);
	}
    }
}

void
dialogs_remove_tab_cmd_callback (GtkWidget *widget,
				 gpointer   data,
				 guint      action)
{
  GimpDockbook *dockbook;

  dockbook = (GimpDockbook *) gtk_item_factory_popup_data_from_widget (widget);

  if (dockbook)
    {
      GimpDockable *dockable;
      gint          page_num;

      page_num = gtk_notebook_get_current_page (GTK_NOTEBOOK (dockbook));

      dockable = (GimpDockable *)
	gtk_notebook_get_nth_page (GTK_NOTEBOOK (dockbook), page_num);

      if (dockable)
	gimp_dockbook_remove (dockbook, dockable);
    }
}

void
dialogs_toggle_view_cmd_callback (GtkWidget *widget,
                                  gpointer   data,
                                  guint      action)
{
  GimpDockbook *dockbook;
  GimpViewType  view_type;

  if (! GTK_CHECK_MENU_ITEM (widget)->active)
    return;

  dockbook = (GimpDockbook *) gtk_item_factory_popup_data_from_widget (widget);

  view_type = (GimpViewType) action;

  if (dockbook)
    {
      GimpDockable *dockable;
      gint          page_num;

      page_num = gtk_notebook_get_current_page (GTK_NOTEBOOK (dockbook));

      dockable = (GimpDockable *)
	gtk_notebook_get_nth_page (GTK_NOTEBOOK (dockbook), page_num);

      if (dockable)
        {
          GimpDialogFactoryEntry *entry;

          entry = g_object_get_data (G_OBJECT (dockable),
                                     "gimp-dialog-factory-entry");

          if (entry)
            {
              gchar *identifier;
              gchar *substring = NULL;

              identifier = g_strdup (entry->identifier);

              if ((substring = strstr (identifier, "grid")) &&
                  view_type == GIMP_VIEW_TYPE_LIST)
                {
                  memcpy (substring, "list", 4);
                }
              else if ((substring = strstr (identifier, "list")) &&
                       view_type == GIMP_VIEW_TYPE_GRID)
                {
                  memcpy (substring, "grid", 4);
                }

              if (substring)
                {
                  GimpContainerView *old_view;
                  GtkWidget         *new_dockable;
                  gint               preview_size = -1;

                  old_view = gimp_container_view_get_by_dockable (dockable);

                  if (old_view)
                    preview_size = old_view->preview_size;

                  new_dockable =
                    gimp_dialog_factory_dockable_new (dockbook->dock->dialog_factory,
                                                      dockbook->dock,
                                                      identifier,
                                                      preview_size);

                  /*  Maybe gimp_dialog_factory_dockable_new() returned
                   *  an already existing singleton dockable, so check
                   *  if it already is attached to a dockbook.
                   */
                  if (new_dockable && ! GIMP_DOCKABLE (new_dockable)->dockbook)
                    {
                      gimp_dockbook_add (dockbook, GIMP_DOCKABLE (new_dockable),
                                         page_num);

                      gimp_dockbook_remove (dockbook, dockable);

                      gtk_notebook_set_current_page (GTK_NOTEBOOK (dockbook),
                                                     page_num);
                    }
                }

              g_free (identifier);
            }
        }
    }
}

void
dialogs_preview_size_cmd_callback (GtkWidget *widget,
                                   gpointer   data,
                                   guint      action)
{
  GimpDockbook *dockbook;
  gint          preview_size;

  if (! GTK_CHECK_MENU_ITEM (widget)->active)
    return;

  dockbook = (GimpDockbook *) gtk_item_factory_popup_data_from_widget (widget);

  preview_size = (gint) action;

  if (dockbook)
    {
      GimpDockable *dockable;
      gint          page_num;

      page_num = gtk_notebook_get_current_page (GTK_NOTEBOOK (dockbook));

      dockable = (GimpDockable *)
	gtk_notebook_get_nth_page (GTK_NOTEBOOK (dockbook), page_num);

      if (dockable)
        {
          GimpContainerView *view;

          view = gimp_container_view_get_by_dockable (dockable);

          if (view)
            gimp_container_view_set_preview_size (view, preview_size);
        }
    }
}

void
dialogs_toggle_image_menu_cmd_callback (GtkWidget *widget,
					gpointer   data,
					guint      action)
{
  GimpDockbook *dockbook;

  dockbook = (GimpDockbook *) gtk_item_factory_popup_data_from_widget (widget);

  if (dockbook && GIMP_IS_IMAGE_DOCK (dockbook->dock))
    {
      gimp_image_dock_set_show_image_menu (GIMP_IMAGE_DOCK (dockbook->dock),
					   GTK_CHECK_MENU_ITEM (widget)->active);
    }
}

void
dialogs_toggle_auto_cmd_callback (GtkWidget *widget,
				  gpointer   data,
				  guint      action)
{
  GimpDockbook *dockbook;

  dockbook = (GimpDockbook *) gtk_item_factory_popup_data_from_widget (widget);

  if (dockbook && GIMP_IS_IMAGE_DOCK (dockbook->dock))
    {
      gimp_image_dock_set_auto_follow_active (GIMP_IMAGE_DOCK (dockbook->dock),
					      GTK_CHECK_MENU_ITEM (widget)->active);
    }
}

void
dialogs_create_lc_cmd_callback (GtkWidget *widget,
                                gpointer   data,
                                guint      action)
{
  GtkWidget *dock;
  GtkWidget *dockbook;
  GtkWidget *dockable;

  const gchar *tabs[] =
  {
    "gimp-layer-list",
    "gimp-channel-list",
    "gimp-vectors-list",
    "gimp-indexed-palette"
  };
  gint i;

  dock = gimp_dialog_factory_dock_new (global_dock_factory);

  gimp_image_dock_set_show_image_menu (GIMP_IMAGE_DOCK (dock), TRUE);

  dockbook = gimp_dockbook_new (global_dock_factory->menu_factory);

  gimp_dock_add_book (GIMP_DOCK (dock), GIMP_DOCKBOOK (dockbook), 0);

  for (i = 0; i < G_N_ELEMENTS (tabs); i++)
    {
      dockable = gimp_dialog_factory_dialog_new (global_dock_factory,
                                                 tabs[i], -1);

      if (dockable && ! GIMP_DOCKABLE (dockable)->dockbook)
        gimp_dock_add (GIMP_DOCK (dock), GIMP_DOCKABLE (dockable), -1, -1);
    }

  gtk_widget_show (dock);
}

void
dialogs_create_stuff_cmd_callback (GtkWidget *widget,
                                   gpointer   data,
                                   guint      action)
{
  GtkWidget *dock;
  GtkWidget *dockbook;
  GtkWidget *dockable;

  const gchar *tabs[] =
  {
    "gimp-brush-grid",
    "gimp-pattern-grid",
    "gimp-gradient-list",
    "gimp-palette-list",
    "gimp-buffer-list",
    "gimp-image-list",
    "gimp-document-history"
  };
  gint i;

  dock = gimp_dialog_factory_dock_new (global_dock_factory);

  dockbook = gimp_dockbook_new (global_dock_factory->menu_factory);

  gimp_dock_add_book (GIMP_DOCK (dock), GIMP_DOCKBOOK (dockbook), 0);

  for (i = 0; i < G_N_ELEMENTS (tabs); i++)
    {
      dockable = gimp_dialog_factory_dialog_new (global_dock_factory,
                                                 tabs[i], -1);

      if (dockable && ! GIMP_DOCKABLE (dockable)->dockbook)
        gimp_dock_add (GIMP_DOCK (dock), GIMP_DOCKABLE (dockable), -1, -1);
    }

  gtk_widget_show (dock);
}

void
dialogs_show_toolbox (void)
{
  if (! global_toolbox_factory->open_dialogs)
    {
      GtkWidget *toolbox;

      toolbox = gimp_dialog_factory_dock_new (global_toolbox_factory);

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
