/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpmenu.c
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <string.h>

#ifdef __GNUC__
#warning GTK_DISABLE_DEPRECATED
#endif
#undef GTK_DISABLE_DEPRECATED

#include "gimp.h"
#include "gimpui.h"

#include "libgimp-intl.h"


#define MENU_THUMBNAIL_WIDTH   24
#define MENU_THUMBNAIL_HEIGHT  24


/*  local function prototypes  */

static GtkWidget * gimp_menu_make_menu     (GimpMenuCallback    callback,
                                            gpointer            data);
static GtkWidget * gimp_menu_add_item      (GtkWidget          *menu,
                                            const gchar        *image_name,
                                            const gchar        *drawable_name,
                                            gint32              any_ID);
static GtkWidget * gimp_menu_add_none      (GtkWidget          *menu);
static GtkWidget * gimp_menu_make_preview  (gint32              any_ID,
                                            gboolean            is_image,
                                            gint                width,
                                            gint                height);
static void        gimp_menu_callback      (GtkWidget          *widget,
                                            gpointer            any_ID);


/*  public functions  */

GtkWidget *
gimp_image_menu_new (GimpConstraintFunc constraint,
		     GimpMenuCallback   callback,
		     gpointer           data,
		     gint32             active_image)
{
  GtkWidget *menu;
  gchar     *name;
  gchar     *label;
  gint32    *images;
  gint32     image = -1;
  gint       n_images;
  gint       i, k;

  g_return_val_if_fail (callback != NULL, NULL);

  menu = gimp_menu_make_menu (callback, data);

  images = gimp_image_list (&n_images);

  for (i = 0, k = 0; i < n_images; i++)
    if (! constraint || (* constraint) (images[i], -1, data))
      {
	name = gimp_image_get_name (images[i]);
	label = g_strdup_printf ("%s-%d", name, images[i]);
	g_free (name);

	gimp_menu_add_item (menu, label, NULL, images[i]);

        g_free (label);

	if (images[i] == active_image)
          {
            image = active_image;
            gtk_menu_set_active (GTK_MENU (menu), k);
          }
        else if (image == -1)
          {
            image = images[i];
          }

	k += 1;
      }

  if (k == 0)
    gimp_menu_add_none (menu);

  (* callback) (image, data);

  g_free (images);

  return menu;
}

GtkWidget *
gimp_layer_menu_new (GimpConstraintFunc constraint,
		     GimpMenuCallback   callback,
		     gpointer           data,
		     gint32             active_layer)
{
  GtkWidget *menu;
  gchar     *image_label;
  gint32    *images;
  gint32    *layers;
  gint32     layer = -1;
  gint       n_images;
  gint       n_layers;
  gint       i, j, k;

  g_return_val_if_fail (callback != NULL, NULL);

  menu = gimp_menu_make_menu (callback, data);

  images = gimp_image_list (&n_images);

  for (i = 0, k = 0; i < n_images; i++)
    if (! constraint || (* constraint) (images[i], -1, data))
      {
        gchar *name;

	name = gimp_image_get_name (images[i]);
	image_label = g_strdup_printf ("%s-%d", name, images[i]);
	g_free (name);

	layers = gimp_image_get_layers (images[i], &n_layers);

	for (j = 0; j < n_layers; j++)
	  if (! constraint || (* constraint) (images[i], layers[j], data))
	    {
	      name = gimp_drawable_get_name (layers[j]);
              gimp_menu_add_item (menu, image_label, name, layers[j]);
	      g_free (name);

	      if (layers[j] == active_layer)
		{
		  layer = active_layer;
		  gtk_menu_set_active (GTK_MENU (menu), k);
		}
	      else if (layer == -1)
                {
                  layer = layers[j];
                }

	      k += 1;
	    }

	g_free (image_label);
        g_free (layers);
      }

  g_free (images);

  if (k == 0)
    gimp_menu_add_none (menu);

  (* callback) (layer, data);

  return menu;
}

GtkWidget *
gimp_channel_menu_new (GimpConstraintFunc constraint,
		       GimpMenuCallback   callback,
		       gpointer           data,
		       gint32             active_channel)
{
  GtkWidget *menu;
  gchar     *image_label;
  gint32    *images;
  gint32    *channels;
  gint32     channel;
  gint       n_images;
  gint       n_channels;
  gint       i, j, k;

  g_return_val_if_fail (callback != NULL, NULL);

  menu = gimp_menu_make_menu (callback, data);

  channel = -1;

  images = gimp_image_list (&n_images);

  for (i = 0, k = 0; i < n_images; i++)
    if (! constraint || (* constraint) (images[i], -1, data))
      {
        gchar *name;

	name = gimp_image_get_name (images[i]);
	image_label = g_strdup_printf ("%s-%d", name, images[i]);
	g_free (name);

	channels = gimp_image_get_channels (images[i], &n_channels);

	for (j = 0; j < n_channels; j++)
	  if (! constraint || (* constraint) (images[i], channels[j], data))
	    {
	      name = gimp_drawable_get_name (channels[j]);
              gimp_menu_add_item (menu, image_label, name, channels[j]);
	      g_free (name);

	      if (channels[j] == active_channel)
		{
		  channel = active_channel;
		  gtk_menu_set_active (GTK_MENU (menu), k);
		}
	      else if (channel == -1)
                {
                  channel = channels[j];
                }

	      k += 1;
	    }

	g_free (image_label);
        g_free (channels);
      }

  g_free (images);

  if (k == 0)
    gimp_menu_add_none (menu);

  (* callback) (channel, data);

  return menu;
}

GtkWidget *
gimp_drawable_menu_new (GimpConstraintFunc constraint,
			GimpMenuCallback   callback,
			gpointer           data,
			gint32             active_drawable)
{
  GtkWidget *menu;
  gchar     *name;
  gchar     *image_label;
  gint32    *images;
  gint32    *layers;
  gint32    *channels;
  gint32     drawable;
  gint       n_images;
  gint       n_layers;
  gint       n_channels;
  gint       i, j, k;

  menu = gimp_menu_make_menu (callback, data);

  drawable = -1;

  images = gimp_image_list (&n_images);

  for (i = 0, k = 0; i < n_images; i++)
    if (! constraint || (* constraint) (images[i], -1, data))
      {
	name = gimp_image_get_name (images[i]);
	image_label = g_strdup_printf ("%s-%d", name, images[i]);
	g_free (name);

	layers   = gimp_image_get_layers   (images[i], &n_layers);
	channels = gimp_image_get_channels (images[i], &n_channels);

	for (j = 0; j < n_layers; j++)
	  if (! constraint || (* constraint) (images[i], layers[j], data))
	    {
	      name = gimp_drawable_get_name (layers[j]);
              gimp_menu_add_item (menu, image_label, name, layers[j]);
	      g_free (name);

	      if (layers[j] == active_drawable)
		{
		  drawable = active_drawable;
		  gtk_menu_set_active (GTK_MENU (menu), k);
		}
	      else if (drawable == -1)
                {
                  drawable = layers[j];
                }

	      k += 1;
	    }

	for (j = 0; j < n_channels; j++)
	  if (! constraint || (* constraint) (images[i], channels[j], data))
	    {
	      name = gimp_drawable_get_name (channels[j]);
              gimp_menu_add_item (menu, image_label, name, channels[j]);
	      g_free (name);

	      if (channels[j] == active_drawable)
		{
		  drawable = active_drawable;
		  gtk_menu_set_active (GTK_MENU (menu), k);
		}
	      else if (drawable == -1)
                {
                  drawable = channels[j];
                }

	      k += 1;
	    }

	g_free (image_label);
        g_free (layers);
        g_free (channels);
      }

  g_free (images);

  if (k == 0)
    gimp_menu_add_none (menu);

  (* callback) (drawable, data);

  return menu;
}


/*  private functions  */

static GtkWidget *
gimp_menu_make_menu (GimpMenuCallback callback,
		     gpointer         data)
{
  GtkWidget *menu;

  menu = gtk_menu_new ();
  g_object_set_data (G_OBJECT (menu), "gimp-menu-callback",      callback);
  g_object_set_data (G_OBJECT (menu), "gimp-menu-callback-data", data);

  return menu;
}

static GtkWidget *
gimp_menu_add_item (GtkWidget   *menu,
                    const gchar *image_name,
                    const gchar *drawable_name,
                    gint32       any_ID)
{
  GtkWidget *menuitem;
  GtkWidget *hbox;
  GtkWidget *vbox;
  GtkWidget *preview;
  GtkWidget *label;
  gchar     *str;

  if (drawable_name)
    str = g_strdup_printf ("%s/%s-%d", image_name, drawable_name, any_ID);
  else
    str = g_strdup (image_name);

  menuitem = gtk_menu_item_new ();
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
  gtk_widget_show (menuitem);

  g_signal_connect (menuitem, "activate",
                    G_CALLBACK (gimp_menu_callback),
                    GINT_TO_POINTER (any_ID));

  hbox = gtk_hbox_new (FALSE, 4);
  gtk_container_add (GTK_CONTAINER (menuitem), hbox);
  gtk_widget_show (hbox);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox, FALSE, FALSE, 0);
  gtk_widget_show (vbox);

  preview = gimp_menu_make_preview (any_ID, drawable_name == NULL,
                                    MENU_THUMBNAIL_WIDTH,
                                    MENU_THUMBNAIL_HEIGHT);
  gtk_container_add (GTK_CONTAINER (vbox), preview);
  gtk_widget_show (preview);

  label = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  g_free (str);

  return menuitem;
}

static GtkWidget *
gimp_menu_add_none (GtkWidget *menu)
{
  GtkWidget *menuitem;

  menuitem = gtk_menu_item_new_with_label (_("None"));
  gtk_widget_set_sensitive (menuitem, FALSE);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
  gtk_widget_show (menuitem);

  return menuitem;
}

static GtkWidget *
gimp_menu_make_preview (gint32     any_ID,
                        gboolean   is_image,
                        gint       width,
                        gint       height)
{
  GtkWidget *preview;
  guchar    *preview_data;
  gint       bpp;
  gint       x, y;
  guchar    *src;
  gdouble    r, g, b, a;
  gdouble    c0, c1;
  guchar    *p0, *p1, *even, *odd;

  bpp = 0; /* Only returned */

  preview = gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_preview_set_dither (GTK_PREVIEW (preview), GDK_RGB_DITHER_MAX);
  gtk_widget_set_size_request (GTK_WIDGET (preview), width, height);

  if (is_image)
    preview_data = gimp_image_get_thumbnail_data (any_ID,
                                                  &width, &height, &bpp);
  else
    preview_data = gimp_drawable_get_thumbnail_data (any_ID,
                                                     &width, &height, &bpp);

  gtk_preview_size (GTK_PREVIEW (preview), width, height);

  even = g_malloc (width * 3);
  odd  = g_malloc (width * 3);
  src  = preview_data;

  for (y = 0; y < height; y++)
    {
      p0 = even;
      p1 = odd;

      for (x = 0; x < width; x++)
	{
	  if (bpp == 4)
	    {
	      r = ((gdouble) src[x*4+0]) / 255.0;
	      g = ((gdouble) src[x*4+1]) / 255.0;
	      b = ((gdouble) src[x*4+2]) / 255.0;
	      a = ((gdouble) src[x*4+3]) / 255.0;
	    }
	  else if (bpp == 3)
	    {
	      r = ((gdouble) src[x*3+0]) / 255.0;
	      g = ((gdouble) src[x*3+1]) / 255.0;
	      b = ((gdouble) src[x*3+2]) / 255.0;
	      a = 1.0;
	    }
	  else
	    {
	      r = g = b = ((gdouble) src[x*bpp+0]) / 255.0;

	      if (bpp == 2)
		a = ((gdouble) src[x*bpp+1]) / 255.0;
	      else
		a = 1.0;
	    }

	  if ((x / GIMP_CHECK_SIZE_SM) & 1)
	    {
	      c0 = GIMP_CHECK_LIGHT;
	      c1 = GIMP_CHECK_DARK;
	    }
	  else
	    {
	      c0 = GIMP_CHECK_DARK;
	      c1 = GIMP_CHECK_LIGHT;
	    }

	  *p0++ = (c0 + (r - c0) * a) * 255.0;
	  *p0++ = (c0 + (g - c0) * a) * 255.0;
	  *p0++ = (c0 + (b - c0) * a) * 255.0;

	  *p1++ = (c1 + (r - c1) * a) * 255.0;
	  *p1++ = (c1 + (g - c1) * a) * 255.0;
	  *p1++ = (c1 + (b - c1) * a) * 255.0;
	}

      if ((y / GIMP_CHECK_SIZE_SM) & 1)
	gtk_preview_draw_row (GTK_PREVIEW (preview), odd,  0, y, width);
      else
	gtk_preview_draw_row (GTK_PREVIEW (preview), even, 0, y, width);

      src += width * bpp;
    }

  g_free (preview_data);
  g_free (even);
  g_free (odd);

  return preview;
}

static void
gimp_menu_callback (GtkWidget *widget,
		    gpointer   any_ID)
{
  GimpMenuCallback callback;
  gpointer         callback_data;

  callback = (GimpMenuCallback) g_object_get_data (G_OBJECT (widget->parent),
                                                   "gimp-menu-callback");
  callback_data = g_object_get_data (G_OBJECT (widget->parent),
                                     "gimp-menu-callback-data");

  (* callback) (GPOINTER_TO_INT (any_ID), callback_data);
}
