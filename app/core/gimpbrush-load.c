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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>

#include <gtk/gtk.h>

#ifdef G_OS_WIN32 /* gets defined by glib.h */
#include <io.h>
#endif

#ifndef _O_BINARY
#define _O_BINARY 0
#endif

#include <stdio.h>

#include "core-types.h"

#include "base/base-config.h"
#include "base/brush-scale.h"
#include "base/temp-buf.h"

#include "gimpbrush.h"
#include "gimpbrush-header.h"
#include "gimpbrushgenerated.h"

/*  this needs to go away  */
#include "tools/tools-types.h"
#include "tools/gimppainttool.h"

#include "libgimp/gimpintl.h"


enum
{
  SPACING_CHANGED,
  LAST_SIGNAL
};


static void        gimp_brush_class_init       (GimpBrushClass *klass);
static void        gimp_brush_init             (GimpBrush      *brush);
static void        gimp_brush_destroy          (GtkObject      *object);
static TempBuf   * gimp_brush_get_new_preview  (GimpViewable   *viewable,
						gint            width,
						gint            height);
static gchar     * gimp_brush_get_extension    (GimpData       *data);

static GimpBrush * gimp_brush_select_brush     (GimpPaintTool  *paint_tool);
static gboolean    gimp_brush_want_null_motion (GimpPaintTool  *paint_tool);


static guint brush_signals[LAST_SIGNAL] = { 0 };

static GimpDataClass *parent_class = NULL;


GtkType
gimp_brush_get_type (void)
{
  static GtkType type = 0;

  if (! type)
    {
      static const GtkTypeInfo info =
      {
        "GimpBrush",
        sizeof (GimpBrush),
        sizeof (GimpBrushClass),
        (GtkClassInitFunc) gimp_brush_class_init,
        (GtkObjectInitFunc) gimp_brush_init,
        /* reserved_1 */ NULL,
        /* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL
      };

    type = gtk_type_unique (GIMP_TYPE_DATA, &info);
  }
  return type;
}

static void
gimp_brush_class_init (GimpBrushClass *klass)
{
  GtkObjectClass    *object_class;
  GimpViewableClass *viewable_class;
  GimpDataClass     *data_class;

  object_class   = (GtkObjectClass *) klass;
  viewable_class = (GimpViewableClass *) klass;
  data_class     = (GimpDataClass *) klass;

  parent_class = gtk_type_class (GIMP_TYPE_DATA);

  brush_signals[SPACING_CHANGED] = 
    gtk_signal_new ("spacing_changed",
                    GTK_RUN_FIRST,
                    object_class->type,
                    GTK_SIGNAL_OFFSET (GimpBrushClass,
                                       spacing_changed),
                    gtk_signal_default_marshaller,
                    GTK_TYPE_NONE, 0);

  gtk_object_class_add_signals (object_class, brush_signals, LAST_SIGNAL);

  object_class->destroy           = gimp_brush_destroy;

  viewable_class->get_new_preview = gimp_brush_get_new_preview;

  data_class->get_extension       = gimp_brush_get_extension;

  klass->select_brush             = gimp_brush_select_brush;
  klass->want_null_motion         = gimp_brush_want_null_motion;
}

static void
gimp_brush_init (GimpBrush *brush)
{
  brush->mask      = NULL;
  brush->pixmap    = NULL;

  brush->spacing   = 20;
  brush->x_axis.x  = 15.0;
  brush->x_axis.y  =  0.0;
  brush->y_axis.x  =  0.0;
  brush->y_axis.y  = 15.0;
}

static void
gimp_brush_destroy (GtkObject *object)
{
  GimpBrush *brush;

  brush = GIMP_BRUSH (object);

  if (brush->mask)
    temp_buf_free (brush->mask);

  if (brush->pixmap)
    temp_buf_free (brush->pixmap);

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

static TempBuf *
gimp_brush_get_new_preview (GimpViewable *viewable,
			    gint          width,
			    gint          height)
{
  GimpBrush   *brush;
  gboolean     scale      = FALSE;
  gint         brush_width;
  gint         brush_height;
  gint         offset_x;
  gint         offset_y;
  TempBuf     *mask_buf   = NULL;
  TempBuf     *pixmap_buf = NULL;
  TempBuf     *return_buf = NULL;
  guchar       white[3] = { 255, 255, 255 };
  guchar      *mask;
  guchar      *buf;
  guchar      *b;
  guchar       bg;
  gint         x, y;

  brush = GIMP_BRUSH (viewable);

  mask_buf     = gimp_brush_get_mask (brush);
  pixmap_buf   = gimp_brush_get_pixmap (brush);

  brush_width  = mask_buf->width;
  brush_height = mask_buf->height;

  if (brush_width > width || brush_height > height)
    {
      gdouble ratio_x = (gdouble) brush_width  / width;
      gdouble ratio_y = (gdouble) brush_height / height;

      brush_width  = (gdouble) brush_width  / MAX (ratio_x, ratio_y) + 0.5;
      brush_height = (gdouble) brush_height / MAX (ratio_x, ratio_y) + 0.5;

      mask_buf = brush_scale_mask (mask_buf, brush_width, brush_height);

      if (pixmap_buf)
        {
          /* TODO: the scale function should scale the pixmap and the
	   *  mask in one run
	   */
          pixmap_buf =
	    brush_scale_pixmap (pixmap_buf, brush_width, brush_height);
        }

      scale = TRUE;
    }

  offset_x = (width  - brush_width)  / 2;
  offset_y = (height - brush_height) / 2;

  return_buf = temp_buf_new (width, height, 3, 0, 0, white);

  mask = temp_buf_data (mask_buf);
  buf  = temp_buf_data (return_buf);

  b = buf + (offset_y * return_buf->width + offset_x) * return_buf->bytes;

  if (pixmap_buf)
    {
      guchar *pixmap = temp_buf_data (pixmap_buf);

      for (y = 0; y < brush_height; y++)
        {
          for (x = 0; x < brush_width ; x++)
            {
              bg = (255 - *mask);

              *b++ = bg + (*mask * *pixmap++) / 255;
              *b++ = bg + (*mask * *pixmap++) / 255;
              *b++ = bg + (*mask * *pixmap++) / 255;

              mask++;
            }

	  b += (return_buf->width - brush_width) * return_buf->bytes;
        }
    }
  else
    {
      for (y = 0; y < brush_height; y++)
        {
          for (x = 0; x < brush_width ; x++)
            {
              bg = 255 - *mask++;

              *b++ = bg;
              *b++ = bg;
              *b++ = bg;
            }

	  b += (return_buf->width - brush_width) * return_buf->bytes;
        }
    }

  if (scale)
    {
      temp_buf_free (mask_buf);

      if (pixmap_buf)
        temp_buf_free (pixmap_buf);
    }

  return return_buf;
}

static gchar *
gimp_brush_get_extension (GimpData *data)
{
  return GIMP_BRUSH_FILE_EXTENSION;
}

GimpData *
gimp_brush_new (const gchar *name)
{
  GimpBrush *brush;

  g_return_val_if_fail (name != NULL, NULL);

  brush = GIMP_BRUSH (gimp_brush_generated_new (5.0, 0.5, 0.0, 1.0));

  gimp_object_set_name (GIMP_OBJECT (brush), name);

  return GIMP_DATA (brush);
}

GimpData *
gimp_brush_get_standard (void)
{
  static GimpBrush *standard_brush = NULL;

  if (! standard_brush)
    {
      standard_brush =
	GIMP_BRUSH (gimp_brush_generated_new (5.0, 0.5, 0.0, 1.0));

      gimp_object_set_name (GIMP_OBJECT (standard_brush), "Standard");

      /*  set ref_count to 2 --> never swap the standard brush  */
      gtk_object_ref (GTK_OBJECT (standard_brush));
      gtk_object_ref (GTK_OBJECT (standard_brush));
      gtk_object_sink (GTK_OBJECT (standard_brush));
    }

  return GIMP_DATA (standard_brush);
}

GimpData *
gimp_brush_load (const gchar *filename)
{
  GimpBrush *brush;
  gint       fd;

  g_return_val_if_fail (filename != NULL, NULL);

  fd = open (filename, O_RDONLY | _O_BINARY);
  if (fd == -1)
    return NULL;

  brush = gimp_brush_load_brush (fd, filename);

  close (fd);

  if (! brush)
    return NULL;

  gimp_data_set_filename (GIMP_DATA (brush), filename);

  /*  Swap the brush to disk (if we're being stingy with memory) */
  if (base_config->stingy_memory_use)
    {
      temp_buf_swap (brush->mask);

      if (brush->pixmap)
	temp_buf_swap (brush->pixmap);
    }

  return GIMP_DATA (brush);
}

static GimpBrush *
gimp_brush_select_brush (GimpPaintTool *paint_core)
{
  return paint_core->brush;
}

static gboolean
gimp_brush_want_null_motion (GimpPaintTool *paint_core)
{
  return TRUE;
}

TempBuf *
gimp_brush_get_mask (const GimpBrush *brush)
{
  g_return_val_if_fail (brush != NULL, NULL);
  g_return_val_if_fail (GIMP_IS_BRUSH (brush), NULL);

  return brush->mask;
}

TempBuf *
gimp_brush_get_pixmap (const GimpBrush *brush)
{
  g_return_val_if_fail (brush != NULL, NULL);
  g_return_val_if_fail (GIMP_IS_BRUSH (brush), NULL);

  return brush->pixmap;
}

gint
gimp_brush_get_spacing (const GimpBrush *brush)
{
  g_return_val_if_fail (brush != NULL, 0);
  g_return_val_if_fail (GIMP_IS_BRUSH (brush), 0);

  return brush->spacing;
}

void
gimp_brush_set_spacing (GimpBrush *brush,
			gint       spacing)
{
  g_return_if_fail (brush != NULL);
  g_return_if_fail (GIMP_IS_BRUSH (brush));

  if (brush->spacing != spacing)
    {
      brush->spacing = spacing;

      gimp_brush_spacing_changed (brush);
    }
}

void
gimp_brush_spacing_changed (GimpBrush *brush)
{
  g_return_if_fail (brush != NULL);
  g_return_if_fail (GIMP_IS_BRUSH (brush));

  gtk_signal_emit (GTK_OBJECT (brush), brush_signals[SPACING_CHANGED]);
}

GimpBrush *
gimp_brush_load_brush (gint         fd,
		       const gchar *filename)
{
  GimpBrush   *brush;
  gint         bn_size;
  BrushHeader  header;
  gchar       *name;
  gint         i;

  g_return_val_if_fail (filename != NULL, NULL);
  g_return_val_if_fail (fd != -1, NULL);

  /*  Read in the header size  */
  if (read (fd, &header, sizeof (header)) != sizeof (header))
    return NULL;

  /*  rearrange the bytes in each unsigned int  */
  header.header_size  = g_ntohl (header.header_size);
  header.version      = g_ntohl (header.version);
  header.width        = g_ntohl (header.width);
  header.height       = g_ntohl (header.height);
  header.bytes        = g_ntohl (header.bytes);
  header.magic_number = g_ntohl (header.magic_number);
  header.spacing      = g_ntohl (header.spacing);

  /*  Check for correct file format */
  /*  It looks as if version 1 did not have the same magic number.  (neo)  */
  if (header.version != 1 &&
      (header.magic_number != GBRUSH_MAGIC || header.version != 2))
    {
      g_message (_("Unknown brush format version #%d in \"%s\"."),
		 header.version, filename);
      return NULL;
    }

  if (header.version == 1)
    {
      /*  If this is a version 1 brush, set the fp back 8 bytes  */
      lseek (fd, -8, SEEK_CUR);
      header.header_size += 8;
      /*  spacing is not defined in version 1  */
      header.spacing = 25;
    }
  
   /*  Read in the brush name  */
  if ((bn_size = (header.header_size - sizeof (header))))
    {
      name = g_new (gchar, bn_size);
      if ((read (fd, name, bn_size)) < bn_size)
	{
	  g_message (_("Error in GIMP brush file \"%s\"."), filename);
	  g_free (name);
	  return NULL;
	}
    }
  else
    {
      name = g_strdup (_("Unnamed"));
    }

  switch (header.bytes)
    {
    case 1:
      brush = GIMP_BRUSH (gtk_type_new (GIMP_TYPE_BRUSH));
      brush->mask = temp_buf_new (header.width, header.height, 1,
				  0, 0, NULL);
      if (read (fd,
		temp_buf_data (brush->mask), header.width * header.height) <
	  header.width * header.height)
	{
	  g_message (_("GIMP brush file appears to be truncated: \"%s\"."),
		     filename);
	  g_free (name);
	  gtk_object_unref (GTK_OBJECT (brush));
	  return NULL;
	}
      break;

    case 4:
      brush = GIMP_BRUSH (gtk_type_new (GIMP_TYPE_BRUSH));
      brush->mask =   temp_buf_new (header.width, header.height, 1, 0, 0, NULL);
      brush->pixmap = temp_buf_new (header.width, header.height, 3, 0, 0, NULL);

      for (i = 0; i < header.width * header.height; i++)
	{
	  if (read (fd, temp_buf_data (brush->pixmap)
		    + i * 3, 3) != 3 ||
	      read (fd, temp_buf_data (brush->mask) + i, 1) != 1)
	    {
	      g_message (_("GIMP brush file appears to be truncated: \"%s\"."),
			 filename);
	      g_free (name);
	      gtk_object_unref (GTK_OBJECT (brush));
	      return NULL;
	    }
	}
      break;

    default:
      g_message ("Unsupported brush depth: %d\n"
		 "in file \"%s\"\n"
		 "GIMP Brushes must be GRAY or RGBA",
		 header.bytes, filename);
      g_free (name);
      return NULL;
    }

  gimp_object_set_name (GIMP_OBJECT (brush), name);

  g_free (name);

  brush->spacing  = header.spacing;
  brush->x_axis.x = header.width  / 2.0;
  brush->x_axis.y = 0.0;
  brush->y_axis.x = 0.0;
  brush->y_axis.y = header.height / 2.0;

  return brush;
}
