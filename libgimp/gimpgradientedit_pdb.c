/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpgradientedit_pdb.c
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

#include "config.h"

#include "gimp.h"

/**
 * gimp_gradient_segment_get_left_color:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @color: The return color.
 * @opacity: The opacity of the endpoint.
 *
 * Retrieves the left endpoint color of the specified gradient and
 * segment
 *
 * This procedure retrieves the left endpoint color of the specified
 * segment of the specified gradient.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_get_left_color (const gchar *name,
				      gint         segment,
				      GimpRGB     *color,
				      gdouble     *opacity)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_get_left_color",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_END);

  *opacity = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *color = return_vals[1].data.d_color;
      *opacity = return_vals[2].data.d_float;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_set_left_color:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @color: The color to set.
 * @opacity: The opacity to set for the endpoint.
 *
 * Retrieves the left endpoint color of the specified gradient and
 * segment
 *
 * This procedure retrieves the left endpoint color of the specified
 * segment of the specified gradient.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_set_left_color (const gchar   *name,
				      gint           segment,
				      const GimpRGB *color,
				      gdouble        opacity)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_set_left_color",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_COLOR, color,
				    GIMP_PDB_FLOAT, opacity,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_get_right_color:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @color: The return color.
 * @opacity: The opacity of the endpoint.
 *
 * Retrieves the right endpoint color of the specified gradient and
 * segment
 *
 * This procedure retrieves the right endpoint color of the specified
 * segment of the specified gradient.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_get_right_color (const gchar *name,
				       gint         segment,
				       GimpRGB     *color,
				       gdouble     *opacity)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_get_right_color",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_END);

  *opacity = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *color = return_vals[1].data.d_color;
      *opacity = return_vals[2].data.d_float;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_set_right_color:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @color: The color to set.
 * @opacity: The opacity to set for the endpoint.
 *
 * Retrieves the right endpoint color of the specified gradient and
 * segment
 *
 * This procedure retrieves the right endpoint color of the specified
 * segment of the specified gradient.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_set_right_color (const gchar   *name,
				       gint           segment,
				       const GimpRGB *color,
				       gdouble        opacity)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_set_right_color",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_COLOR, color,
				    GIMP_PDB_FLOAT, opacity,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_get_left_pos:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @pos: The return position.
 *
 * Retrieves the left endpoint position of the specified gradient and
 * segment
 *
 * This procedure retrieves the left endpoint position of the specified
 * segment of the specified gradient.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_get_left_pos (const gchar *name,
				    gint         segment,
				    gdouble     *pos)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_get_left_pos",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_END);

  *pos = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *pos = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_set_left_pos:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @pos: The position to set the guidepoint in.
 * @final_pos: The return position.
 *
 * Sets the left endpoint position of the specified gradient and
 * segment
 *
 * This procedure sets the left endpoint position of the specified
 * segment of the specified gradient. The final position will be
 * between the position of the middle point to the left to the middle
 * point of the current segement. This procedure returns the final
 * position.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_set_left_pos (const gchar *name,
				    gint         segment,
				    gdouble      pos,
				    gdouble     *final_pos)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_set_left_pos",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_FLOAT, pos,
				    GIMP_PDB_END);

  *final_pos = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *final_pos = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_get_middle_pos:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @pos: The return position.
 *
 * Retrieves the middle point position of the specified gradient and
 * segment
 *
 * This procedure retrieves the middle point position of the specified
 * segment of the specified gradient.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_get_middle_pos (const gchar *name,
				      gint         segment,
				      gdouble     *pos)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_get_middle_pos",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_END);

  *pos = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *pos = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_set_middle_pos:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @pos: The position to set the guidepoint in.
 * @final_pos: The return position.
 *
 * Sets the middle point position of the specified gradient and segment
 *
 * This procedure sets the middle point position of the specified
 * segment of the specified gradient. The final position will be
 * between the two endpoints of the segment. This procedure returns the
 * final position.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_set_middle_pos (const gchar *name,
				      gint         segment,
				      gdouble      pos,
				      gdouble     *final_pos)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_set_middle_pos",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_FLOAT, pos,
				    GIMP_PDB_END);

  *final_pos = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *final_pos = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_get_right_pos:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @pos: The return position.
 *
 * Retrieves the right endpoint position of the specified gradient and
 * segment
 *
 * This procedure retrieves the right endpoint position of the
 * specified segment of the specified gradient.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_get_right_pos (const gchar *name,
				     gint         segment,
				     gdouble     *pos)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_get_right_pos",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_END);

  *pos = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *pos = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_set_right_pos:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @pos: The position to set the guidepoint in.
 * @final_pos: The return position.
 *
 * Sets the right endpoint position of the specified gradient and
 * segment
 *
 * This procedure sets the right endpoint position of the specified
 * segment of the specified gradient. The final position will be
 * between the position of the middle point of the current segment and
 * the middle point of the segment to the right. This procedure returns
 * the final position.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_set_right_pos (const gchar *name,
				     gint         segment,
				     gdouble      pos,
				     gdouble     *final_pos)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_set_right_pos",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_FLOAT, pos,
				    GIMP_PDB_END);

  *final_pos = 0.0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *final_pos = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_get_blending_function:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @blend_func: The blending function of the segment.
 *
 * Retrieves the gradient segment's blending function
 *
 * This procedure retrieves the blending function of the segment at the
 * specified gradient name and segment index.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_get_blending_function (const gchar             *name,
					     gint                     segment,
					     GimpGradientSegmentType *blend_func)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_get_blending_function",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_END);

  *blend_func = 0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *blend_func = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_set_blending_function:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 * @blending_function: The Blending Function.
 *
 * Change the blending function of a segments range
 *
 * This function changes the blending function of a segment range to
 * the specified blending function.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_set_blending_function (const gchar             *name,
					     gint                     start_segment,
					     gint                     end_segment,
					     GimpGradientSegmentType  blending_function)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_set_blending_function",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_INT32, blending_function,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_get_coloring_type:
 * @name: The name of the gradient to operate on.
 * @segment: The index of the segment within the gradient.
 * @coloring_type: The coloring type of the segment.
 *
 * Retrieves the gradient segment's coloring type
 *
 * This procedure retrieves the coloring type of the segment at the
 * specified gradient name and segment index.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_get_coloring_type (const gchar              *name,
					 gint                      segment,
					 GimpGradientSegmentColor *coloring_type)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_get_coloring_type",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, segment,
				    GIMP_PDB_END);

  *coloring_type = 0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *coloring_type = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_set_coloring_type:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 * @coloring_type: The Coloring Type.
 *
 * Change the coloring type of a segments range
 *
 * This function changes the coloring type of a segment range to the
 * specified coloring type.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_set_coloring_type (const gchar              *name,
					 gint                      start_segment,
					 gint                      end_segment,
					 GimpGradientSegmentColor  coloring_type)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_set_coloring_type",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_INT32, coloring_type,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_flip:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 *
 * Flip the segment range
 *
 * This function flips a segment range.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_range_flip (const gchar *name,
				  gint         start_segment,
				  gint         end_segment)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_flip",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_replicate:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 * @replicate_times: The number of times to replicate.
 *
 * Replicate the segment range
 *
 * This function replicates a segment range a given number of times.
 * Instead of the original segment range, several smaller scaled copies
 * of it will appear in equal widths.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_range_replicate (const gchar *name,
				       gint         start_segment,
				       gint         end_segment,
				       gint         replicate_times)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_replicate",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_INT32, replicate_times,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_split_midpoint:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 *
 * Splits each segment in the segment range at midpoint
 *
 * This function splits each segment in the segment range at its
 * midpoint.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_range_split_midpoint (const gchar *name,
					    gint         start_segment,
					    gint         end_segment)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_split_midpoint",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_split_uniform:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 * @split_parts: The number of uniform divisions to split each segment to.
 *
 * Splits each segment in the segment range uniformly
 *
 * This function splits each segment in the segment range uniformly
 * according to the number of times specified by the parameter.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_range_split_uniform (const gchar *name,
					   gint         start_segment,
					   gint         end_segment,
					   gint         split_parts)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_split_uniform",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_INT32, split_parts,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_delete:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 *
 * Delete the segment range
 *
 * This function deletes a segment range.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_range_delete (const gchar *name,
				    gint         start_segment,
				    gint         end_segment)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_delete",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_redistribute_handles:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 *
 * Uniformly redistribute the segment range's handles
 *
 * This function redistributes the handles of the specified segment
 * range of the specified gradient, so they'll be evenly spaced.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_range_redistribute_handles (const gchar *name,
						  gint         start_segment,
						  gint         end_segment)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_redistribute_handles",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_blend_colors:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 *
 * Blend the colors of the segment range.
 *
 * This function blends the colors (but not the opacity) of the
 * segments' range of the gradient. Using it, the colors' transition
 * will be uniform across the range.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_range_blend_colors (const gchar *name,
					  gint         start_segment,
					  gint         end_segment)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_blend_colors",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_blend_opacity:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 *
 * Blend the opacity of the segment range.
 *
 * This function blends the opacity (but not the colors) of the
 * segments' range of the gradient. Using it, the opacity's transition
 * will be uniform across the range.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradient_segment_range_blend_opacity (const gchar *name,
					   gint         start_segment,
					   gint         end_segment)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_blend_opacity",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradient_segment_range_move:
 * @name: The name of the gradient to operate on.
 * @start_segment: The index of the first segment to operate on.
 * @end_segment: The index of the last segment to operate on. If negative, the selection will extend to the end of the string.
 * @delta: The delta to move the segment range.
 * @control_compress: Whether or not to compress the neighboring segments.
 *
 * Move the position of an entire segment range by a delta.
 *
 * This funtions moves the position of an entire segment range by a
 * delta. The actual delta (which is returned) will be limited by the
 * control points of the neighboring segments.
 *
 * Returns: The final delta by which the range moved.
 */
gdouble
gimp_gradient_segment_range_move (const gchar *name,
				  gint         start_segment,
				  gint         end_segment,
				  gdouble      delta,
				  gboolean     control_compress)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gdouble final_delta = 0;

  return_vals = gimp_run_procedure ("gimp_gradient_segment_range_move",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_INT32, start_segment,
				    GIMP_PDB_INT32, end_segment,
				    GIMP_PDB_FLOAT, delta,
				    GIMP_PDB_INT32, control_compress,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    final_delta = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return final_delta;
}
