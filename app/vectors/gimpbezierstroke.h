/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpbezierstroke.h
 * Copyright (C) 2002 Simon Budig  <simon@gimp.org>
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

#ifndef __GIMP_BEZIER_STROKE_H__
#define __GIMP_BEZIER_STROKE_H__

#include "config.h"

#include "glib-object.h"

#include "vectors-types.h"

#include "gimpstroke.h"


#define GIMP_TYPE_BEZIER_STROKE            (gimp_bezier_stroke_get_type ())
#define GIMP_BEZIER_STROKE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_BEZIER_STROKE, GimpBezierStroke))
#define GIMP_BEZIER_STROKE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_BEZIER_STROKE, GimpBezierStrokeClass))
#define GIMP_IS_BEZIER_STROKE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_BEZIER_STROKE))
#define GIMP_IS_BEZIER_STROKE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_BEZIER_STROKE))
#define GIMP_BEZIER_STROKE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_BEZIER_STROKE, GimpBezierStrokeClass))


struct _GimpBezierStroke
{
  GimpStroke        parent_instance;

  /* Stuff missing? */
};


struct _GimpBezierStrokeClass
{
  GimpStrokeClass     parent_class;
};


GType              gimp_bezier_stroke_get_type          (void) G_GNUC_CONST;
                
                
GimpStroke       * gimp_bezier_stroke_new               (const GimpCoords   *start);


#endif /* __GIMP_BEZIER_STROKE_H__ */
