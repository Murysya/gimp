/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimppixmap.h
 * Copyright (C) 2000 Michael Natterer <mitch@gimp.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#ifndef __GIMP_PIXMAP_H__
#define __GIMP_PIXMAP_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GIMP_TYPE_PIXMAP            (gimp_pixmap_get_type ())
#define GIMP_PIXMAP(obj)            (GTK_CHECK_CAST ((obj), GIMP_TYPE_PIXMAP, GimpPixmap))
#define GIMP_PIXMAP_CLASS(klass)    (GTK_CHECK_CLASS_CAST ((klass), GIMP_TYPE_PIXMAP, GimpPixmapClass))
#define GIMP_IS_PIXMAP(obj)         (GTK_CHECK_TYPE ((obj), GIMP_TYPE_PIXMAP))
#define GIMP_IS_PIXMAP_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_PIXMAP))

typedef struct _GimpPixmap       GimpPixmap;
typedef struct _GimpPixmapClass  GimpPixmapClass;

struct _GimpPixmapClass
{
  GtkPixmapClass parent_class;
};

GtkType     gimp_pixmap_get_type (void);
GtkWidget * gimp_pixmap_new      (gchar      **xpm_data);

void        gimp_pixmap_set      (GimpPixmap  *pixmap,
				  gchar      **xpm_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __GIMP_PIXMAP_H__ */
