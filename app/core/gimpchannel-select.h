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

#ifndef __GIMP_IMAGE_MASK_SELECT_H__
#define __GIMP_IMAGE_MASK_SELECT_H__


void   gimp_image_mask_select_rectangle (GimpImage      *gimage,
                                         gint            x,
                                         gint            y,
                                         gint            w,
                                         gint            h,
                                         GimpChannelOps  op,
                                         gboolean        feather,
                                         gdouble         feather_radius_x,
                                         gdouble         feather_radius_y);

void   gimp_image_mask_select_ellipse   (GimpImage      *gimage,
                                         gint            x,
                                         gint            y,
                                         gint            w,
                                         gint            h,
                                         GimpChannelOps  op,
                                         gboolean        antialias,
                                         gboolean        feather,
                                         gdouble         feather_radius_x,
                                         gdouble         feather_radius_y);

void   gimp_image_mask_select_polygon   (GimpImage      *gimage,
                                         const gchar    *undo_desc,
                                         gint            n_points,
                                         GimpVector2    *points,
                                         GimpChannelOps  op,
                                         gboolean        antialias,
                                         gboolean        feather,
                                         gdouble         feather_radius_x,
                                         gdouble         feather_radius_y);

void   gimp_image_mask_select_vectors   (GimpImage      *gimage,
                                         GimpVectors    *vectors,
                                         GimpChannelOps  op,
                                         gboolean        antialias,
                                         gboolean        feather,
                                         gdouble         feather_radius_x,
                                         gdouble         feather_radius_y);

void   gimp_image_mask_select_channel   (GimpImage      *gimage,
                                         const gchar    *undo_desc,
                                         GimpChannel    *channel,
                                         gint            offset_x,
                                         gint            offset_y,
                                         GimpChannelOps  op,
                                         gboolean        feather,
                                         gdouble         feather_radius_x,
                                         gdouble         feather_radius_y);

void   gimp_image_mask_select_fuzzy     (GimpImage      *gimage,
                                         GimpDrawable   *drawable,
                                         gboolean        sample_merged,
                                         gint            x,
                                         gint            y,
                                         gint            threshold,
                                         gboolean        select_transparent,
                                         GimpChannelOps  op,
                                         gboolean        antialias,
                                         gboolean        feather,
                                         gdouble         feather_radius_x,
                                         gdouble         feather_radius_y);

void   gimp_image_mask_select_by_color  (GimpImage      *gimage,
                                         GimpDrawable   *drawable,
                                         gboolean        sample_merged,
                                         const GimpRGB  *color,
                                         gint            threshold,
                                         gboolean        select_transparent,
                                         GimpChannelOps  op,
                                         gboolean        antialias,
                                         gboolean        feather,
                                         gdouble         feather_radius_x,
                                         gdouble         feather_radius_y);


#endif  /*  __GIMP_IMAGE_MASK_SELECT_H__  */
