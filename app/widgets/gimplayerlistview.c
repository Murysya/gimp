/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimplayerlistview.c
 * Copyright (C) 2001 Michael Natterer
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

#include <gtk/gtk.h>

#include "libgimpmath/gimpmath.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "widgets-types.h"

#include "core/gimpcontainer.h"
#include "core/gimpdrawable.h"
#include "core/gimplayer.h"
#include "core/gimpmarshal.h"

#include "drawable.h"
#include "gdisplay.h"
#include "gimplistitem.h"
#include "gimprc.h"

#include "gimpdnd.h"
#include "gimplayerlistview.h"

/* TODO: move the paint mode menu constructor to widgets/ */
#include "tools/tools-types.h"
#include "tools/paint_options.h"

#include "libgimp/gimpintl.h"

#include "pixmaps/anchor.xpm"


static void   gimp_layer_list_view_class_init (GimpLayerListViewClass *klass);
static void   gimp_layer_list_view_init       (GimpLayerListView      *view);

static void   gimp_layer_list_view_destroy        (GtkObject         *object);

static void   gimp_layer_list_view_set_container  (GimpContainerView *view,
						   GimpContainer     *container);
static void   gimp_layer_list_view_select_item    (GimpContainerView *view,
						   GimpViewable      *item,
						   gpointer           insert_data);

static void   gimp_layer_list_view_anchor_layer   (GimpLayerListView *view,
						   GimpLayer         *layer);
static void   gimp_layer_list_view_anchor_clicked (GtkWidget         *widget,
						   GimpLayerListView *view);
static void   gimp_layer_list_view_anchor_dropped (GtkWidget         *widget,
						   GimpViewable      *viewable,
						   gpointer           data);

static void   gimp_layer_list_view_paint_mode_menu_callback
                                                  (GtkWidget         *widget,
						   GimpLayerListView *view);
static void   gimp_layer_list_view_preserve_button_toggled
                                                  (GtkWidget         *widget,
						   GimpLayerListView *view);
static void   gimp_layer_list_view_opacity_scale_changed
                                                  (GtkAdjustment     *adjustment,
						   GimpLayerListView *view);

static void   gimp_layer_list_view_layer_signal_handler
                                                  (GimpLayer         *layer,
						   GimpLayerListView *view);
static void   gimp_layer_list_view_update_options (GimpLayerListView *view,
						   GimpLayer         *layer);


static GimpDrawableListViewClass *parent_class = NULL;


GtkType
gimp_layer_list_view_get_type (void)
{
  static guint view_type = 0;

  if (! view_type)
    {
      GtkTypeInfo view_info =
      {
	"GimpLayerListView",
	sizeof (GimpLayerListView),
	sizeof (GimpLayerListViewClass),
	(GtkClassInitFunc) gimp_layer_list_view_class_init,
	(GtkObjectInitFunc) gimp_layer_list_view_init,
	/* reserved_1 */ NULL,
	/* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL
      };

      view_type = gtk_type_unique (GIMP_TYPE_DRAWABLE_LIST_VIEW, &view_info);
    }

  return view_type;
}

static void
gimp_layer_list_view_class_init (GimpLayerListViewClass *klass)
{
  GtkObjectClass         *object_class;
  GimpContainerViewClass *container_view_class;

  object_class         = (GtkObjectClass *) klass;
  container_view_class = (GimpContainerViewClass *) klass;

  parent_class = gtk_type_class (GIMP_TYPE_DRAWABLE_LIST_VIEW);

  object_class->destroy               = gimp_layer_list_view_destroy;

  container_view_class->set_container = gimp_layer_list_view_set_container;
  container_view_class->select_item   = gimp_layer_list_view_select_item;
}

static void
gimp_layer_list_view_init (GimpLayerListView *view)
{
  GimpDrawableListView *drawable_view;
  GtkWidget            *hbox;
  GtkWidget            *label;
  GtkWidget            *slider;
  GtkWidget            *pixmap;

  drawable_view = GIMP_DRAWABLE_LIST_VIEW (view);

  view->options_box = gtk_vbox_new (FALSE, 2);
  gtk_box_pack_start (GTK_BOX (view), view->options_box, FALSE, FALSE, 0);
  gtk_box_reorder_child (GTK_BOX (view), view->options_box, 0);
  gtk_widget_show (view->options_box);

  hbox = gtk_hbox_new (FALSE, 2);
  gtk_box_pack_start (GTK_BOX (view->options_box), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  /*  Paint mode menu  */

  label = gtk_label_new (_("Mode:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  view->paint_mode_menu =
    paint_mode_menu_new (gimp_layer_list_view_paint_mode_menu_callback, view,
			 FALSE, NORMAL_MODE);
  gtk_box_pack_start (GTK_BOX (hbox), view->paint_mode_menu, FALSE, FALSE, 2);
  gtk_widget_show (view->paint_mode_menu);

  gimp_help_set_help_data (view->paint_mode_menu, 
			   NULL, "#paint_mode_menu");

  /*  Preserve transparency toggle  */

  view->preserve_trans_toggle =
    gtk_check_button_new_with_label (_("Keep Trans."));
  gtk_box_pack_start (GTK_BOX (hbox), view->preserve_trans_toggle,
		      FALSE, FALSE, 2);
  gtk_widget_show (view->preserve_trans_toggle);

  gtk_signal_connect (GTK_OBJECT (view->preserve_trans_toggle), "toggled",
		      GTK_SIGNAL_FUNC (gimp_layer_list_view_preserve_button_toggled),
		      view);

  gimp_help_set_help_data (view->preserve_trans_toggle,
			   _("Keep Transparency"), "#keep_trans_button");

  hbox = gtk_hbox_new (FALSE, 2);
  gtk_box_pack_start (GTK_BOX (view->options_box), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  /*  Opacity scale  */

  label = gtk_label_new (_("Opacity:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  view->opacity_adjustment =
    GTK_ADJUSTMENT (gtk_adjustment_new (100.0, 0.0, 100.0, 1.0, 1.0, 0.0));

  gtk_signal_connect (GTK_OBJECT (view->opacity_adjustment), "value_changed",
		      GTK_SIGNAL_FUNC (gimp_layer_list_view_opacity_scale_changed),
		      view);

  slider = gtk_hscale_new (view->opacity_adjustment);
  gtk_scale_set_value_pos (GTK_SCALE (slider), GTK_POS_RIGHT);
  gtk_box_pack_start (GTK_BOX (hbox), slider, TRUE, TRUE, 0);
  gtk_widget_show (slider);

  gimp_help_set_help_data (slider, NULL, "#opacity_scale");

  /*  Anchor button  */

  view->anchor_button = gtk_button_new ();
  gtk_box_pack_start (GTK_BOX (drawable_view->button_box), view->anchor_button,
		      TRUE, TRUE, 0);
  gtk_box_reorder_child (GTK_BOX (drawable_view->button_box),
			 view->anchor_button, 5);
  gtk_widget_show (view->anchor_button);

  gimp_help_set_help_data (view->anchor_button, _("Anchor"), NULL);

  gtk_signal_connect (GTK_OBJECT (view->anchor_button), "clicked",
		      GTK_SIGNAL_FUNC (gimp_layer_list_view_anchor_clicked),
		      view);

  pixmap = gimp_pixmap_new (anchor_xpm);
  gtk_container_add (GTK_CONTAINER (view->anchor_button), pixmap);
  gtk_widget_show (pixmap);

  gimp_gtk_drag_dest_set_by_type (GTK_WIDGET (view->anchor_button),
				  GTK_DEST_DEFAULT_ALL,
				  GIMP_TYPE_LAYER,
				  GDK_ACTION_COPY);
  gimp_dnd_viewable_dest_set (GTK_WIDGET (view->anchor_button),
			      GIMP_TYPE_LAYER,
			      gimp_layer_list_view_anchor_dropped,
			      view);

  gtk_widget_set_sensitive (view->options_box,   FALSE);
  gtk_widget_set_sensitive (view->anchor_button, FALSE);

  view->mode_changed_handler_id           = 0;
  view->opacity_changed_handler_id        = 0;
  view->preserve_trans_changed_handler_id = 0;
}

static void
gimp_layer_list_view_destroy (GtkObject *object)
{
  GimpLayerListView *view;

  view = GIMP_LAYER_LIST_VIEW (object);

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    GTK_OBJECT_CLASS (parent_class)->destroy (object);
}


/*  GimpContainerView methods  */

static void
gimp_layer_list_view_set_container (GimpContainerView *view,
				    GimpContainer     *container)
{
  GimpLayerListView *layer_view;

  layer_view = GIMP_LAYER_LIST_VIEW (view);

  if (view->container)
    {
      gimp_container_remove_handler (view->container,
				     layer_view->mode_changed_handler_id);
      gimp_container_remove_handler (view->container,
				     layer_view->opacity_changed_handler_id);
      gimp_container_remove_handler (view->container,
				     layer_view->preserve_trans_changed_handler_id);
    }

  if (GIMP_CONTAINER_VIEW_CLASS (parent_class)->set_container)
    GIMP_CONTAINER_VIEW_CLASS (parent_class)->set_container (view, container);

  if (view->container)
    {
      layer_view->mode_changed_handler_id =
	gimp_container_add_handler (view->container, "mode_changed",
				    gimp_layer_list_view_layer_signal_handler,
				    view);
      layer_view->opacity_changed_handler_id =
	gimp_container_add_handler (view->container, "opacity_changed",
				    gimp_layer_list_view_layer_signal_handler,
				    view);
      layer_view->preserve_trans_changed_handler_id =
	gimp_container_add_handler (view->container, "preserve_trans_changed",
				    gimp_layer_list_view_layer_signal_handler,
				    view);
    }
}

static void
gimp_layer_list_view_select_item (GimpContainerView *view,
				  GimpViewable      *item,
				  gpointer           insert_data)
{
  GimpDrawableListView *drawable_view;
  GimpLayerListView    *list_view;
  gboolean              options_sensitive   = FALSE;
  gboolean              anchor_sensitive    = FALSE;
  gboolean              raise_sensitive     = FALSE;
  gboolean              lower_sensitive     = FALSE;
  gboolean              duplicate_sensitive = FALSE;
  gboolean              edit_sensitive      = FALSE;

  list_view     = GIMP_LAYER_LIST_VIEW (view);
  drawable_view = GIMP_DRAWABLE_LIST_VIEW (view);

  if (GIMP_CONTAINER_VIEW_CLASS (parent_class)->select_item)
    GIMP_CONTAINER_VIEW_CLASS (parent_class)->select_item (view,
							   item,
							   insert_data);

  if (item)
    {
      gimp_layer_list_view_update_options (list_view, GIMP_LAYER (item));

      options_sensitive = TRUE;

      if (gimp_layer_is_floating_sel (GIMP_LAYER (item)))
	{
	  anchor_sensitive = TRUE;
	}
      else
	{
	  lower_sensitive     = TRUE;
	  duplicate_sensitive = TRUE;
	  edit_sensitive      = TRUE;

	  if (gimp_drawable_has_alpha (GIMP_DRAWABLE (item)) &&
	      gimp_container_get_child_index (view->container,
					      GIMP_OBJECT (item)))
	    {
	      raise_sensitive = TRUE;
	    }
	}
    }

  gtk_widget_set_sensitive (list_view->options_box,          options_sensitive);
  gtk_widget_set_sensitive (drawable_view->raise_button,     raise_sensitive);
  gtk_widget_set_sensitive (drawable_view->lower_button,     lower_sensitive);
  gtk_widget_set_sensitive (drawable_view->duplicate_button, duplicate_sensitive);
  gtk_widget_set_sensitive (drawable_view->edit_button,      edit_sensitive);
  gtk_widget_set_sensitive (list_view->anchor_button,        anchor_sensitive);
}


/*  "Anchor" functions  */

static void
gimp_layer_list_view_anchor_layer (GimpLayerListView *view,
				   GimpLayer         *layer)
{
  if (layer)
    g_print ("anchor \"%s\"\n", GIMP_OBJECT (layer)->name);
}

static void
gimp_layer_list_view_anchor_clicked (GtkWidget         *widget,
				     GimpLayerListView *view)
{
  GimpDrawableListView *drawable_view;
  GimpDrawable         *drawable;

  drawable_view = GIMP_DRAWABLE_LIST_VIEW (view);

  drawable = drawable_view->get_drawable_func (drawable_view->gimage);

  if (drawable)
    gimp_layer_list_view_anchor_layer (view, GIMP_LAYER (drawable));
}

static void
gimp_layer_list_view_anchor_dropped (GtkWidget    *widget,
				     GimpViewable *viewable,
				     gpointer      data)
{
  GimpLayerListView *view;

  view = (GimpLayerListView *) data;

  if (viewable && gimp_container_have (GIMP_CONTAINER_VIEW (view)->container,
				       GIMP_OBJECT (viewable)))
    {
      gimp_layer_list_view_anchor_layer (view, GIMP_LAYER (viewable));
    }
}


/*  Paint Mode, Opacity and Preservce trans. callbacks  */

#define BLOCK() \
        gtk_signal_handler_block_by_func (GTK_OBJECT (layer), \
	gimp_layer_list_view_layer_signal_handler, view)

#define UNBLOCK() \
        gtk_signal_handler_unblock_by_func (GTK_OBJECT (layer), \
	gimp_layer_list_view_layer_signal_handler, view)


static void
gimp_layer_list_view_paint_mode_menu_callback (GtkWidget         *widget,
					       GimpLayerListView *view)
{
  GimpDrawableListView *drawable_view;
  GimpLayer            *layer;

  drawable_view = GIMP_DRAWABLE_LIST_VIEW (view);

  layer = (GimpLayer *) drawable_view->get_drawable_func (drawable_view->gimage);

  if (layer)
    {
      LayerModeEffects mode;

      mode = (LayerModeEffects)
	GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (widget)));

      if (gimp_layer_get_mode (layer) != mode)
	{
	  BLOCK();
	  gimp_layer_set_mode (layer, mode);
	  UNBLOCK();

	  drawable_update (GIMP_DRAWABLE (layer), 0, 0,
			   GIMP_DRAWABLE (layer)->width,
			   GIMP_DRAWABLE (layer)->height);

	  gdisplays_flush ();
	}
    }
}

static void
gimp_layer_list_view_preserve_button_toggled (GtkWidget         *widget,
					      GimpLayerListView *view)
{
  GimpDrawableListView *drawable_view;
  GimpLayer            *layer;

  drawable_view = GIMP_DRAWABLE_LIST_VIEW (view);

  layer = (GimpLayer *) drawable_view->get_drawable_func (drawable_view->gimage);

  if (layer)
    {
      gboolean preserve_trans;

      preserve_trans = GTK_TOGGLE_BUTTON (widget)->active;

      if (gimp_layer_get_preserve_trans (layer) != preserve_trans)
	{
	  BLOCK();
	  gimp_layer_set_preserve_trans (layer, preserve_trans);
	  UNBLOCK();
	}
    }
}

static void
gimp_layer_list_view_opacity_scale_changed (GtkAdjustment     *adjustment,
					    GimpLayerListView *view)
{
  GimpDrawableListView *drawable_view;
  GimpLayer            *layer;

  drawable_view = GIMP_DRAWABLE_LIST_VIEW (view);

  layer = (GimpLayer *) drawable_view->get_drawable_func (drawable_view->gimage);

  if (layer)
    {
      gdouble opacity;

      opacity = adjustment->value / 100.0;

      if (gimp_layer_get_opacity (layer) != opacity)
	{
	  BLOCK();
	  gimp_layer_set_opacity (layer, opacity);
	  UNBLOCK();

	  drawable_update (GIMP_DRAWABLE (layer), 0, 0,
			   GIMP_DRAWABLE (layer)->width,
			   GIMP_DRAWABLE (layer)->height);

	  gdisplays_flush ();
	}
    }
}

#undef BLOCK
#undef UNBLOCK


static void
gimp_layer_list_view_layer_signal_handler (GimpLayer         *layer,
					   GimpLayerListView *view)
{
  GimpDrawableListView *drawable_view;

  drawable_view = GIMP_DRAWABLE_LIST_VIEW (view);

  if (drawable_view->get_drawable_func (drawable_view->gimage) ==
      (GimpDrawable *) layer)
    {
      gimp_layer_list_view_update_options (view, layer);
    }
}


#define BLOCK(object,function) \
        gtk_signal_handler_block_by_func (GTK_OBJECT (object), \
	                                  (function), view)

#define UNBLOCK(object,function) \
        gtk_signal_handler_unblock_by_func (GTK_OBJECT (object), \
	                                    (function), view)

static void
gimp_layer_list_view_update_options (GimpLayerListView *view,
				     GimpLayer         *layer)
{
  gimp_option_menu_set_history (GTK_OPTION_MENU (view->paint_mode_menu),
				GINT_TO_POINTER (layer->mode));

  if (layer->preserve_trans !=
      GTK_TOGGLE_BUTTON (view->preserve_trans_toggle)->active)
    {
      BLOCK (view->preserve_trans_toggle,
	     gimp_layer_list_view_preserve_button_toggled);

      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (view->preserve_trans_toggle),
				    layer->preserve_trans);

      UNBLOCK (view->preserve_trans_toggle,
	       gimp_layer_list_view_preserve_button_toggled);
      }

  if ((gdouble) layer->opacity / 2.55 != view->opacity_adjustment->value)
    {
      BLOCK (view->opacity_adjustment,
	     gimp_layer_list_view_opacity_scale_changed);

      gtk_adjustment_set_value (view->opacity_adjustment, layer->opacity / 2.55);

      UNBLOCK (view->opacity_adjustment,
	       gimp_layer_list_view_opacity_scale_changed);
    }
}

#undef BLOCK
#undef UNBLOCK
