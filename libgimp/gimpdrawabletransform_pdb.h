/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpdrawabletransform_pdb.h
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

/* NOTE: This file is autogenerated by pdbgen.pl */

#ifndef __GIMP_DRAWABLE_TRANSFORM_PDB_H__
#define __GIMP_DRAWABLE_TRANSFORM_PDB_H__

G_BEGIN_DECLS

/* For information look into the C source or the html documentation */


gint32 gimp_drawable_transform_flip        (gint32                 drawable_ID,
					    GimpOrientationType    flip_type,
					    gboolean               center,
					    gdouble                axis,
					    GimpTransformDirection transform_direction,
					    GimpInterpolationType  interpolation,
					    gboolean               supersample,
					    gint                   recursion_level,
					    gboolean               clip_result);
gint32 gimp_drawable_transform_flip_free   (gint32                 drawable_ID,
					    gdouble                x0,
					    gdouble                y0,
					    gdouble                x1,
					    gdouble                y1,
					    GimpTransformDirection transform_direction,
					    GimpInterpolationType  interpolation,
					    gboolean               supersample,
					    gint                   recursion_level,
					    gboolean               clip_result);
gint32 gimp_drawable_transform_perspective (gint32                 drawable_ID,
					    gdouble                x0,
					    gdouble                y0,
					    gdouble                x1,
					    gdouble                y1,
					    gdouble                x2,
					    gdouble                y2,
					    gdouble                x3,
					    gdouble                y3,
					    GimpTransformDirection transform_direction,
					    GimpInterpolationType  interpolation,
					    gboolean               supersample,
					    gint                   recursion_level,
					    gboolean               clip_result);
gint32 gimp_drawable_transform_rotate      (gint32                 drawable_ID,
					    gdouble                angle,
					    gint                   center_x,
					    gint                   center_y,
					    GimpTransformDirection transform_direction,
					    GimpInterpolationType  interpolation,
					    gboolean               supersample,
					    gint                   recursion_level,
					    gboolean               clip_result);
gint32 gimp_drawable_transform_scale       (gint32                 drawable_ID,
					    gdouble                x0,
					    gdouble                y0,
					    gdouble                x1,
					    gdouble                y1,
					    GimpTransformDirection transform_direction,
					    GimpInterpolationType  interpolation,
					    gboolean               supersample,
					    gint                   recursion_level,
					    gboolean               clip_result);
gint32 gimp_drawable_transform_shear       (gint32                 drawable_ID,
					    GimpOrientationType    shear_type,
					    gdouble                magnitude,
					    GimpTransformDirection transform_direction,
					    GimpInterpolationType  interpolation,
					    gboolean               supersample,
					    gint                   recursion_level,
					    gboolean               clip_result);
gint32 gimp_drawable_transform_2d          (gint32                 drawable_ID,
					    gdouble                source_x,
					    gdouble                source_y,
					    gdouble                scale_x,
					    gdouble                scale_y,
					    gdouble                angle,
					    gdouble                dest_x,
					    gdouble                dest_y,
					    GimpTransformDirection transform_direction,
					    GimpInterpolationType  interpolation,
					    gboolean               supersample,
					    gint                   recursion_level,
					    gboolean               clip_result);
gint32 gimp_drawable_transform_matrix      (gint32                 drawable_ID,
					    gdouble                coeff_0_0,
					    gdouble                coeff_0_1,
					    gdouble                coeff_0_2,
					    gdouble                coeff_1_0,
					    gdouble                coeff_1_1,
					    gdouble                coeff_1_2,
					    gdouble                coeff_2_0,
					    gdouble                coeff_2_1,
					    gdouble                coeff_2_2,
					    GimpTransformDirection transform_direction,
					    GimpInterpolationType  interpolation,
					    gboolean               supersample,
					    gint                   recursion_level,
					    gboolean               clip_result);


G_END_DECLS

#endif /* __GIMP_DRAWABLE_TRANSFORM_PDB_H__ */
