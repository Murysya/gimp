/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpbrushfactoryview.h
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

#ifndef __GIMP_BRUSH_FACTORY_VIEW_H__
#define __GIMP_BRUSH_FACTORY_VIEW_H__


#include "gimpdatafactoryview.h"


#define GIMP_TYPE_BRUSH_FACTORY_VIEW            (gimp_brush_factory_view_get_type ())
#define GIMP_BRUSH_FACTORY_VIEW(obj)            (GTK_CHECK_CAST ((obj), GIMP_TYPE_BRUSH_FACTORY_VIEW, GimpBrushFactoryView))
#define GIMP_BRUSH_FACTORY_VIEW_CLASS(klass)    (GTK_CHECK_CLASS_CAST ((klass), GIMP_TYPE_BRUSH_FACTORY_VIEW, GimpBrushFactoryViewClass))
#define GIMP_IS_BRUSH_FACTORY_VIEW(obj)         (GTK_CHECK_TYPE ((obj), GIMP_TYPE_BRUSH_FACTORY_VIEW))
#define GIMP_IS_BRUSH_FACTORY_VIEW_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_BRUSH_FACTORY_VIEW))


typedef struct _GimpBrushFactoryViewClass  GimpBrushFactoryViewClass;

struct _GimpBrushFactoryView
{
  GimpDataFactoryView  parent_instance;

  GtkWidget           *spacing_scale;
  GtkAdjustment       *spacing_adjustment;

  gboolean             change_brush_spacing;
  GQuark               spacing_changed_handler_id;
};

struct _GimpBrushFactoryViewClass
{
  GimpDataFactoryViewClass  parent_class;
};


GtkType     gimp_brush_factory_view_get_type (void);

GtkWidget * gimp_brush_factory_view_new      (GimpViewType      view_type,
					      GimpDataFactory  *factory,
					      GimpDataEditFunc  edit_func,
					      GimpContext      *context,
					      gboolean          change_brush_spacing,
					      gint              preview_size,
					      gint              min_items_x,
					      gint              min_items_y);


#endif  /*  __GIMP_BRUSH_FACTORY_VIEW_H__  */
