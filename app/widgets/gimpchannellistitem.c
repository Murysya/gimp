/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpchannellistitem.c
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

#include "libgimpcolor/gimpcolor.h"

#include "widgets-types.h"

#include "core/gimpchannel.h"

#include "gimpdnd.h"
#include "gimpchannellistitem.h"
#include "gimppreview.h"


static void   gimp_channel_list_item_class_init (GimpChannelListItemClass *klass);
static void   gimp_channel_list_item_init       (GimpChannelListItem      *list_item);

/*
static void   gimp_channel_list_item_drop_color (GtkWidget     *widget,
						 const GimpRGB *color,
						 gpointer       data);
*/


GType
gimp_channel_list_item_get_type (void)
{
  static GType list_item_type = 0;

  if (! list_item_type)
    {
      static const GtkTypeInfo list_item_info =
      {
	"GimpChannelListItem",
	sizeof (GimpChannelListItem),
	sizeof (GimpChannelListItemClass),
	(GtkClassInitFunc) gimp_channel_list_item_class_init,
	(GtkObjectInitFunc) gimp_channel_list_item_init,
	/* reserved_1 */ NULL,
        /* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      list_item_type = gtk_type_unique (GIMP_TYPE_DRAWABLE_LIST_ITEM,
                                        &list_item_info);
    }

  return list_item_type;
}

static void
gimp_channel_list_item_class_init (GimpChannelListItemClass *klass)
{
}

static void
gimp_channel_list_item_init (GimpChannelListItem *list_item)
{
  /*
  static GtkTargetEntry color_target_table[] =
  {
    GIMP_TARGET_COLOR
  };
  static guint n_color_targets = (sizeof (color_target_table) /
				  sizeof (color_target_table[0]));

  gtk_drag_dest_set (GTK_WIDGET (list_item),
                     GTK_DEST_DEFAULT_HIGHLIGHT |
                     GTK_DEST_DEFAULT_MOTION    |
                     GTK_DEST_DEFAULT_DROP,
                     color_target_table, n_color_targets,
                     GDK_ACTION_COPY);

  gimp_dnd_color_dest_set (GTK_WIDGET (list_item),
			   gimp_channel_list_item_drop_color, NULL);
  */
}

/*
static void
gimp_channel_list_item_drop_color (GtkWidget     *widget,
				   const GimpRGB *color,
				   gpointer       data)
{
  GimpChannel *channel;

  channel =
    GIMP_CHANNEL (GIMP_PREVIEW (GIMP_LIST_ITEM (widget)->preview)->viewable);

  if (gimp_rgba_distance (color, &channel->color) > 0.0001)
    {
      channel->color = *color;

      drawable_update (GIMP_DRAWABLE (channel),
		       0, 0,
		       GIMP_DRAWABLE (channel)->width,
		       GIMP_DRAWABLE (channel)->height);

      gdisplays_flush ();
    }
}
*/
