/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is autogenerated by pdbgen.pl. */

#include "config.h"


#include <glib-object.h>

#include "libgimpmath/gimpmath.h"

#include "pdb-types.h"
#include "gimpprocedure.h"
#include "procedural_db.h"
#include "core/gimpparamspecs.h"

#include "config/gimpcoreconfig.h"
#include "core/gimp-transform-utils.h"
#include "core/gimp.h"
#include "core/gimpdrawable-transform.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "core/gimpprogress.h"
#include "gimp-intl.h"

static ProcRecord flip_proc;
static ProcRecord perspective_proc;
static ProcRecord rotate_proc;
static ProcRecord scale_proc;
static ProcRecord shear_proc;
static ProcRecord transform_2d_proc;

void
register_transform_tools_procs (Gimp *gimp)
{
  ProcRecord *procedure;

  /*
   * flip
   */
  procedure = gimp_procedure_init (&flip_proc, 2, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The affected drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_enum ("flip-type",
                                                     "flip type",
                                                     "Type of flip: { GIMP_ORIENTATION_HORIZONTAL (0), GIMP_ORIENTATION_VERTICAL (1) }",
                                                     GIMP_TYPE_ORIENTATION_TYPE,
                                                     GIMP_ORIENTATION_HORIZONTAL,
                                                     GIMP_PARAM_READWRITE));
  gimp_param_spec_enum_exclude_value (GIMP_PARAM_SPEC_ENUM (procedure->args[1].pspec),
                                      GIMP_ORIENTATION_UNKNOWN);
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_DRAWABLE,
                                   gimp_param_spec_item_id ("drawable",
                                                            "drawable",
                                                            "The flipped drawable",
                                                            gimp,
                                                            GIMP_TYPE_DRAWABLE,
                                                            GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * perspective
   */
  procedure = gimp_procedure_init (&perspective_proc, 10, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The affected drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("interpolation",
                                                     "interpolation",
                                                     "Whether to use interpolation",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("x0",
                                                    "x0",
                                                    "The new x coordinate of upper-left corner of original bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("y0",
                                                    "y0",
                                                    "The new y coordinate of upper-left corner of original bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("x1",
                                                    "x1",
                                                    "The new x coordinate of upper-right corner of original bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("y1",
                                                    "y1",
                                                    "The new y coordinate of upper-right corner of original bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("x2",
                                                    "x2",
                                                    "The new x coordinate of lower-left corner of original bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("y2",
                                                    "y2",
                                                    "The new y coordinate of lower-left corner of original bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("x3",
                                                    "x3",
                                                    "The new x coordinate of lower-right corner of original bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("y3",
                                                    "y3",
                                                    "The new y coordinate of lower-right corner of original bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_DRAWABLE,
                                   gimp_param_spec_item_id ("drawable",
                                                            "drawable",
                                                            "The newly mapped drawable",
                                                            gimp,
                                                            GIMP_TYPE_DRAWABLE,
                                                            GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * rotate
   */
  procedure = gimp_procedure_init (&rotate_proc, 3, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The affected drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("interpolation",
                                                     "interpolation",
                                                     "Whether to use interpolation",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("angle",
                                                    "angle",
                                                    "The angle of rotation (radians)",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_DRAWABLE,
                                   gimp_param_spec_item_id ("drawable",
                                                            "drawable",
                                                            "The rotated drawable",
                                                            gimp,
                                                            GIMP_TYPE_DRAWABLE,
                                                            GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * scale
   */
  procedure = gimp_procedure_init (&scale_proc, 6, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The affected drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("interpolation",
                                                     "interpolation",
                                                     "Whether to use interpolation",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("x0",
                                                    "x0",
                                                    "The new x coordinate of the upper-left corner of the scaled region",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("y0",
                                                    "y0",
                                                    "The new y coordinate of the upper-left corner of the scaled region",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("x1",
                                                    "x1",
                                                    "The new x coordinate of the lower-right corner of the scaled region",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("y1",
                                                    "y1",
                                                    "The new y coordinate of the lower-right corner of the scaled region",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_DRAWABLE,
                                   gimp_param_spec_item_id ("drawable",
                                                            "drawable",
                                                            "The scaled drawable",
                                                            gimp,
                                                            GIMP_TYPE_DRAWABLE,
                                                            GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * shear
   */
  procedure = gimp_procedure_init (&shear_proc, 4, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The affected drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("interpolation",
                                                     "interpolation",
                                                     "Whether to use interpolation",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               gimp_param_spec_enum ("shear-type",
                                                     "shear type",
                                                     "Type of shear: { GIMP_ORIENTATION_HORIZONTAL (0), GIMP_ORIENTATION_VERTICAL (1) }",
                                                     GIMP_TYPE_ORIENTATION_TYPE,
                                                     GIMP_ORIENTATION_HORIZONTAL,
                                                     GIMP_PARAM_READWRITE));
  gimp_param_spec_enum_exclude_value (GIMP_PARAM_SPEC_ENUM (procedure->args[2].pspec),
                                      GIMP_ORIENTATION_UNKNOWN);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("magnitude",
                                                    "magnitude",
                                                    "The magnitude of the shear",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_DRAWABLE,
                                   gimp_param_spec_item_id ("drawable",
                                                            "drawable",
                                                            "The sheared drawable",
                                                            gimp,
                                                            GIMP_TYPE_DRAWABLE,
                                                            GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * transform_2d
   */
  procedure = gimp_procedure_init (&transform_2d_proc, 9, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The affected drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_INT32,
                               g_param_spec_boolean ("interpolation",
                                                     "interpolation",
                                                     "Whether to use interpolation",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("source-x",
                                                    "source x",
                                                    "X coordinate of the transformation center",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("source-y",
                                                    "source y",
                                                    "Y coordinate of the transformation center",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("scale-x",
                                                    "scale x",
                                                    "Amount to scale in x direction",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("scale-y",
                                                    "scale y",
                                                    "Amount to scale in y direction",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("angle",
                                                    "angle",
                                                    "The angle of rotation (radians)",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("dest-x",
                                                    "dest x",
                                                    "X coordinate of where the centre goes",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_FLOAT,
                               g_param_spec_double ("dest-y",
                                                    "dest y",
                                                    "Y coordinate of where the centre goes",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_DRAWABLE,
                                   gimp_param_spec_item_id ("drawable",
                                                            "drawable",
                                                            "The transformed drawable",
                                                            gimp,
                                                            GIMP_TYPE_DRAWABLE,
                                                            GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static Argument *
flip_invoker (ProcRecord   *proc_record,
              Gimp         *gimp,
              GimpContext  *context,
              GimpProgress *progress,
              Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpDrawable *drawable;
  gint32 flip_type;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  flip_type = g_value_get_enum (&args[1].value);

  if (success)
    {
      gint x, y, width, height;

      success = gimp_item_is_attached (GIMP_ITEM (drawable));

      if (success &&
          gimp_drawable_mask_intersect (drawable, &x, &y, &width, &height))
        {
          success = gimp_drawable_transform_flip (drawable, context,
                                                  flip_type, TRUE, 0.0, FALSE);
        }
    }

  return_vals = gimp_procedure_get_return_values (proc_record, success);

  if (success)
    gimp_value_set_item (&return_vals[1].value, GIMP_ITEM (drawable));

  return return_vals;
}

static ProcRecord flip_proc =
{
  TRUE,
  "gimp-flip",
  "gimp-flip",
  "This procedure is deprecated! Use 'gimp-drawable-transform-flip-simple' instead.",
  "This procedure is deprecated! Use 'gimp-drawable-transform-flip-simple' instead.",
  "",
  "",
  "",
  "gimp-drawable-transform-flip-simple",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { flip_invoker } }
};

static Argument *
perspective_invoker (ProcRecord   *proc_record,
                     Gimp         *gimp,
                     GimpContext  *context,
                     GimpProgress *progress,
                     Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpDrawable *drawable;
  gboolean interpolation;
  gdouble x0;
  gdouble y0;
  gdouble x1;
  gdouble y1;
  gdouble x2;
  gdouble y2;
  gdouble x3;
  gdouble y3;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  interpolation = g_value_get_boolean (&args[1].value);
  x0 = g_value_get_double (&args[2].value);
  y0 = g_value_get_double (&args[3].value);
  x1 = g_value_get_double (&args[4].value);
  y1 = g_value_get_double (&args[5].value);
  x2 = g_value_get_double (&args[6].value);
  y2 = g_value_get_double (&args[7].value);
  x3 = g_value_get_double (&args[8].value);
  y3 = g_value_get_double (&args[9].value);

  if (success)
    {
      gint x, y, width, height;

      success = gimp_item_is_attached (GIMP_ITEM (drawable));

      if (success &&
          gimp_drawable_mask_intersect (drawable, &x, &y, &width, &height))
        {
          GimpMatrix3           matrix;
          GimpInterpolationType interpolation_type = GIMP_INTERPOLATION_NONE;

          /* Assemble the transformation matrix */
          gimp_matrix3_identity (&matrix);
          gimp_transform_matrix_perspective (&matrix,
                                             x, y, width, height,
                                             x0, y0,
                                             x1, y1,
                                             x2, y2,
                                             x3, y3);

          if (interpolation)
            interpolation_type = gimp->config->interpolation_type;

          if (progress)
            gimp_progress_start (progress, _("Perspective"), FALSE);

          /* Perspective the selection */
          success = gimp_drawable_transform_affine (drawable, context,
                                                    &matrix,
                                                    GIMP_TRANSFORM_FORWARD,
                                                    interpolation_type, TRUE, 3,
                                                    FALSE, progress);

          if (progress)
            gimp_progress_end (progress);
        }
    }

  return_vals = gimp_procedure_get_return_values (proc_record, success);

  if (success)
    gimp_value_set_item (&return_vals[1].value, GIMP_ITEM (drawable));

  return return_vals;
}

static ProcRecord perspective_proc =
{
  TRUE,
  "gimp-perspective",
  "gimp-perspective",
  "This procedure is deprecated! Use 'gimp-drawable-transform-perspective-default' instead.",
  "This procedure is deprecated! Use 'gimp-drawable-transform-perspective-default' instead.",
  "",
  "",
  "",
  "gimp-drawable-transform-perspective-default",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { perspective_invoker } }
};

static Argument *
rotate_invoker (ProcRecord   *proc_record,
                Gimp         *gimp,
                GimpContext  *context,
                GimpProgress *progress,
                Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpDrawable *drawable;
  gboolean interpolation;
  gdouble angle;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  interpolation = g_value_get_boolean (&args[1].value);
  angle = g_value_get_double (&args[2].value);

  if (success)
    {
      gint x, y, width, height;

      success = gimp_item_is_attached (GIMP_ITEM (drawable));

      if (success &&
          gimp_drawable_mask_intersect (drawable, &x, &y, &width, &height))
        {
          GimpMatrix3           matrix;
          GimpInterpolationType interpolation_type = GIMP_INTERPOLATION_NONE;

          /* Assemble the transformation matrix */
          gimp_matrix3_identity (&matrix);
          gimp_transform_matrix_rotate_rect (&matrix,
                                             x, y, width, height, angle);

          if (interpolation)
            interpolation_type = gimp->config->interpolation_type;

          if (progress)
            gimp_progress_start (progress, _("Rotating"), FALSE);

          /* Rotate the selection */
          success = gimp_drawable_transform_affine (drawable, context,
                                                    &matrix,
                                                    GIMP_TRANSFORM_FORWARD,
                                                    interpolation_type, FALSE, 3,
                                                    FALSE, progress);

          if (progress)
            gimp_progress_end (progress);
        }
    }

  return_vals = gimp_procedure_get_return_values (proc_record, success);

  if (success)
    gimp_value_set_item (&return_vals[1].value, GIMP_ITEM (drawable));

  return return_vals;
}

static ProcRecord rotate_proc =
{
  TRUE,
  "gimp-rotate",
  "gimp-rotate",
  "This procedure is deprecated! Use 'gimp-drawable-transform-rotate-default' instead.",
  "This procedure is deprecated! Use 'gimp-drawable-transform-rotate-default' instead.",
  "",
  "",
  "",
  "gimp-drawable-transform-rotate-default",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { rotate_invoker } }
};

static Argument *
scale_invoker (ProcRecord   *proc_record,
               Gimp         *gimp,
               GimpContext  *context,
               GimpProgress *progress,
               Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpDrawable *drawable;
  gboolean interpolation;
  gdouble x0;
  gdouble y0;
  gdouble x1;
  gdouble y1;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  interpolation = g_value_get_boolean (&args[1].value);
  x0 = g_value_get_double (&args[2].value);
  y0 = g_value_get_double (&args[3].value);
  x1 = g_value_get_double (&args[4].value);
  y1 = g_value_get_double (&args[5].value);

  if (success)
    {
      gint x, y, width, height;

      success = (gimp_item_is_attached (GIMP_ITEM (drawable)) &&
                 x0 < x1 && y0 < y1);

      if (success &&
          gimp_drawable_mask_intersect (drawable, &x, &y, &width, &height))
        {
          GimpMatrix3           matrix;
          GimpInterpolationType interpolation_type = GIMP_INTERPOLATION_NONE;

          /* Assemble the transformation matrix */
          gimp_matrix3_identity (&matrix);
          gimp_transform_matrix_scale (&matrix,
                                       x, y, width, height,
                                       x0, y0,
                                       x1 - x0,
                                       y1 - y0);

          if (interpolation)
            interpolation_type = gimp->config->interpolation_type;

          if (progress)
            gimp_progress_start (progress, _("Scaling"), FALSE);

          /* Scale the selection */
          success = gimp_drawable_transform_affine (drawable, context,
                                                    &matrix,
                                                    GIMP_TRANSFORM_FORWARD,
                                                    interpolation_type, TRUE, 3,
                                                    FALSE, progress);

          if (progress)
            gimp_progress_end (progress);
        }
    }

  return_vals = gimp_procedure_get_return_values (proc_record, success);

  if (success)
    gimp_value_set_item (&return_vals[1].value, GIMP_ITEM (drawable));

  return return_vals;
}

static ProcRecord scale_proc =
{
  TRUE,
  "gimp-scale",
  "gimp-scale",
  "This procedure is deprecated! Use 'gimp-drawable-transform-scale-default' instead.",
  "This procedure is deprecated! Use 'gimp-drawable-transform-scale-default' instead.",
  "",
  "",
  "",
  "gimp-drawable-transform-scale-default",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { scale_invoker } }
};

static Argument *
shear_invoker (ProcRecord   *proc_record,
               Gimp         *gimp,
               GimpContext  *context,
               GimpProgress *progress,
               Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpDrawable *drawable;
  gboolean interpolation;
  gint32 shear_type;
  gdouble magnitude;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  interpolation = g_value_get_boolean (&args[1].value);
  shear_type = g_value_get_enum (&args[2].value);
  magnitude = g_value_get_double (&args[3].value);

  if (success)
    {
      gint x, y, width, height;

      success = gimp_item_is_attached (GIMP_ITEM (drawable));

      if (success &&
          gimp_drawable_mask_intersect (drawable, &x, &y, &width, &height))
        {
          GimpMatrix3           matrix;
          GimpInterpolationType interpolation_type = GIMP_INTERPOLATION_NONE;

          /* Assemble the transformation matrix */
          gimp_matrix3_identity (&matrix);
          gimp_transform_matrix_shear (&matrix,
                                       x, y, width, height,
                                       shear_type, magnitude);

          if (interpolation)
            interpolation_type = gimp->config->interpolation_type;

          if (progress)
            gimp_progress_start (progress, _("Shearing"), FALSE);

          /* Shear the selection */
          success = gimp_drawable_transform_affine (drawable, context,
                                                    &matrix,
                                                    GIMP_TRANSFORM_FORWARD,
                                                    interpolation_type, FALSE, 3,
                                                    FALSE, progress);

          if (progress)
            gimp_progress_end (progress);
        }
    }

  return_vals = gimp_procedure_get_return_values (proc_record, success);

  if (success)
    gimp_value_set_item (&return_vals[1].value, GIMP_ITEM (drawable));

  return return_vals;
}

static ProcRecord shear_proc =
{
  TRUE,
  "gimp-shear",
  "gimp-shear",
  "This procedure is deprecated! Use 'gimp-drawable-transform-shear-default' instead.",
  "This procedure is deprecated! Use 'gimp-drawable-transform-shear-default' instead.",
  "",
  "",
  "",
  "gimp-drawable-transform-shear-default",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { shear_invoker } }
};

static Argument *
transform_2d_invoker (ProcRecord   *proc_record,
                      Gimp         *gimp,
                      GimpContext  *context,
                      GimpProgress *progress,
                      Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpDrawable *drawable;
  gboolean interpolation;
  gdouble source_x;
  gdouble source_y;
  gdouble scale_x;
  gdouble scale_y;
  gdouble angle;
  gdouble dest_x;
  gdouble dest_y;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  interpolation = g_value_get_boolean (&args[1].value);
  source_x = g_value_get_double (&args[2].value);
  source_y = g_value_get_double (&args[3].value);
  scale_x = g_value_get_double (&args[4].value);
  scale_y = g_value_get_double (&args[5].value);
  angle = g_value_get_double (&args[6].value);
  dest_x = g_value_get_double (&args[7].value);
  dest_y = g_value_get_double (&args[8].value);

  if (success)
    {
      gint x, y, width, height;

      success = gimp_item_is_attached (GIMP_ITEM (drawable));

      if (success &&
          gimp_drawable_mask_intersect (drawable, &x, &y, &width, &height))
        {
          GimpMatrix3           matrix;
          GimpInterpolationType interpolation_type = GIMP_INTERPOLATION_NONE;

          /* Assemble the transformation matrix */
          gimp_matrix3_identity  (&matrix);
          gimp_matrix3_translate (&matrix, -source_x, -source_y);
          gimp_matrix3_scale     (&matrix, scale_x, scale_y);
          gimp_matrix3_rotate    (&matrix, angle);
          gimp_matrix3_translate (&matrix, dest_x, dest_y);

          if (interpolation)
            interpolation_type = gimp->config->interpolation_type;

          if (progress)
            gimp_progress_start (progress, _("2D Transform"), FALSE);

          /* Transform the selection */
          success = gimp_drawable_transform_affine (drawable, context,
                                                    &matrix, GIMP_TRANSFORM_FORWARD,
                                                    interpolation_type, TRUE, 3,
                                                    FALSE, progress);

          if (progress)
            gimp_progress_end (progress);
        }
    }

  return_vals = gimp_procedure_get_return_values (proc_record, success);

  if (success)
    gimp_value_set_item (&return_vals[1].value, GIMP_ITEM (drawable));

  return return_vals;
}

static ProcRecord transform_2d_proc =
{
  TRUE,
  "gimp-transform-2d",
  "gimp-transform-2d",
  "This procedure is deprecated! Use 'gimp-drawable-transform-2d-default' instead.",
  "This procedure is deprecated! Use 'gimp-drawable-transform-2d-default' instead.",
  "",
  "",
  "",
  "gimp-drawable-transform-2d-default",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { transform_2d_invoker } }
};
