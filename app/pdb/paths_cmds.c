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

#include <string.h>

#include <glib-object.h>

#include "libgimpbase/gimpbasetypes.h"

#include "pdb-types.h"
#include "procedural_db.h"

#include "core/gimp.h"
#include "core/gimpcontext.h"
#include "core/gimpimage-mask-select.h"
#include "core/gimpimage.h"
#include "core/gimplist.h"
#include "core/gimptoolinfo.h"
#include "gimp-intl.h"
#include "vectors/gimpanchor.h"
#include "vectors/gimpbezierstroke.h"
#include "vectors/gimpvectors-compat.h"
#include "vectors/gimpvectors.h"

static ProcRecord path_list_proc;
static ProcRecord path_get_current_proc;
static ProcRecord path_set_current_proc;
static ProcRecord path_delete_proc;
static ProcRecord path_get_points_proc;
static ProcRecord path_set_points_proc;
static ProcRecord path_stroke_current_proc;
static ProcRecord path_get_point_at_dist_proc;
static ProcRecord path_get_tattoo_proc;
static ProcRecord path_set_tattoo_proc;
static ProcRecord get_path_by_tattoo_proc;
static ProcRecord path_get_locked_proc;
static ProcRecord path_set_locked_proc;
static ProcRecord path_to_selection_proc;

void
register_paths_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &path_list_proc);
  procedural_db_register (gimp, &path_get_current_proc);
  procedural_db_register (gimp, &path_set_current_proc);
  procedural_db_register (gimp, &path_delete_proc);
  procedural_db_register (gimp, &path_get_points_proc);
  procedural_db_register (gimp, &path_set_points_proc);
  procedural_db_register (gimp, &path_stroke_current_proc);
  procedural_db_register (gimp, &path_get_point_at_dist_proc);
  procedural_db_register (gimp, &path_get_tattoo_proc);
  procedural_db_register (gimp, &path_set_tattoo_proc);
  procedural_db_register (gimp, &get_path_by_tattoo_proc);
  procedural_db_register (gimp, &path_get_locked_proc);
  procedural_db_register (gimp, &path_set_locked_proc);
  procedural_db_register (gimp, &path_to_selection_proc);
}

static Argument *
path_list_invoker (Gimp     *gimp,
                   Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gint32 num_paths;
  gchar **path_list = NULL;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  if (success)
    path_list = gimp_container_get_name_array (gimage->vectors, &num_paths);

  return_args = procedural_db_return_args (&path_list_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = num_paths;
      return_args[2].value.pdb_pointer = path_list;
    }

  return return_args;
}

static ProcArg path_list_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The ID of the image to list the paths from"
  }
};

static ProcArg path_list_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "num_paths",
    "The number of paths returned"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "path_list",
    "List of the paths belonging to this image"
  }
};

static ProcRecord path_list_proc =
{
  "gimp_path_list",
  "List the paths associated with the passed image.",
  "List the paths associated with the passed image.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  1,
  path_list_inargs,
  2,
  path_list_outargs,
  { { path_list_invoker } }
};

static Argument *
path_get_current_invoker (Gimp     *gimp,
                          Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gchar *name = NULL;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  if (success)
    {
      vectors = gimp_image_get_active_vectors (gimage);
    
      if (vectors)
	name = g_strdup (gimp_object_get_name (GIMP_OBJECT (vectors)));
      else
	success = FALSE;
    }

  return_args = procedural_db_return_args (&path_get_current_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = name;

  return return_args;
}

static ProcArg path_get_current_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The ID of the image to get the current paths from"
  }
};

static ProcArg path_get_current_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "name",
    "The name of the current path"
  }
};

static ProcRecord path_get_current_proc =
{
  "gimp_path_get_current",
  "The name of the current path. Error if no paths.",
  "The name of the current path. Error if no paths.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  1,
  path_get_current_inargs,
  1,
  path_get_current_outargs,
  { { path_get_current_invoker } }
};

static Argument *
path_set_current_invoker (Gimp     *gimp,
                          Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  gchar *name;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_name (gimage, name);
    
      if (vectors)
	gimp_image_set_active_vectors (gimage, vectors);
      else
	success = FALSE;
    }

  return procedural_db_return_args (&path_set_current_proc, success);
}

static ProcArg path_set_current_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The ID of the image to list set the paths in"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "The name of the path to set the current path to"
  }
};

static ProcRecord path_set_current_proc =
{
  "gimp_path_set_current",
  "List the paths associated with the passed image.",
  "List the paths associated with the passed image.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  2,
  path_set_current_inargs,
  0,
  NULL,
  { { path_set_current_invoker } }
};

static Argument *
path_delete_invoker (Gimp     *gimp,
                     Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  gchar *name;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_name (gimage, name);
    
      if (vectors)
	gimp_image_remove_vectors (gimage, vectors);
      else
	success = FALSE;
    }

  return procedural_db_return_args (&path_delete_proc, success);
}

static ProcArg path_delete_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The ID of the image to list delete the paths from"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "The name of the path to delete"
  }
};

static ProcRecord path_delete_proc =
{
  "gimp_path_delete",
  "Delete the named paths associated with the passed image.",
  "Delete the named path.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  2,
  path_delete_inargs,
  0,
  NULL,
  { { path_delete_invoker } }
};

static Argument *
path_get_points_invoker (Gimp     *gimp,
                         Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gchar *name;
  gint32 path_type = 0;
  gint32 path_closed = 0;
  gint32 num_points = 0;
  gdouble *points_pairs = NULL;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_name (gimage, name);
    
      if (vectors)
	{
	  GimpVectorsCompatPoint *points;
    
	  path_type = 1; /* BEZIER (1.2 compat) */
    
	  points = gimp_vectors_compat_get_points (vectors, &num_points,
						   &path_closed);
    
	  if (points)
	    {
	      gdouble *curr_point;
	      gint     i;
    
	      points_pairs = g_new0 (gdouble, num_points * 3);
    
	      for (i = 0, curr_point = points_pairs;
		   i < num_points;
		   i++, curr_point += 3)
		{
		  curr_point[0] = points[i].x;
		  curr_point[1] = points[i].y;
		  curr_point[2] = points[i].type;
		}
    
	      g_free (points);
	    }
	  else
	    success = FALSE;
	}
      else
	success = FALSE;
    }

  return_args = procedural_db_return_args (&path_get_points_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = path_type;
      return_args[2].value.pdb_int = path_closed;
      return_args[3].value.pdb_int = num_points;
      return_args[4].value.pdb_pointer = points_pairs;
    }

  return return_args;
}

static ProcArg path_get_points_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The ID of the image to list the paths from"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "the name of the path whose points should be listed"
  }
};

static ProcArg path_get_points_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "path_type",
    "The type of the path. Currently only one type (1 = Bezier) is supported"
  },
  {
    GIMP_PDB_INT32,
    "path_closed",
    "Return if the path is closed. (0 = path open, 1 = path closed)"
  },
  {
    GIMP_PDB_INT32,
    "num_path_point_details",
    "The number of points returned. Each point is made up of (x, y, pnt_type) of floats"
  },
  {
    GIMP_PDB_FLOATARRAY,
    "points_pairs",
    "The points in the path represented as 3 floats. The first is the x pos, next is the y pos, last is the type of the pnt. The type field is dependant on the path type. For beziers (type 1 paths) the type can either be (1.0 = BEZIER_ANCHOR, 2.0 = BEZIER_CONTROL, 3.0 = BEZIER_MOVE). Note all points are returned in pixel resolution"
  }
};

static ProcRecord path_get_points_proc =
{
  "gimp_path_get_points",
  "List the points associated with the named path.",
  "List the points associated with the named path.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  2,
  path_get_points_inargs,
  4,
  path_get_points_outargs,
  { { path_get_points_invoker } }
};

static Argument *
path_set_points_invoker (Gimp     *gimp,
                         Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  gchar *name = NULL;
  gint32 ptype;
  gint32 num_path_points = 0;
  gdouble *points_pairs;
  gboolean closed = FALSE;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  ptype = args[2].value.pdb_int;

  num_path_points = args[3].value.pdb_int;
  if (num_path_points <= 0)
    success = FALSE;

  points_pairs = (gdouble *) args[4].value.pdb_pointer;

  if (success)
    {
      if ((num_path_points / 3) % 3 == 0)
	closed = TRUE;
      else if ((num_path_points / 3) % 3 != 2)
	success = FALSE;
    
      if (success)
	{
	  GimpVectors            *vectors;
	  gdouble                *curr_point_pair;
	  GimpVectorsCompatPoint *points;
	  gint                    n_points;
	  gint                    i;
    
	  n_points = num_path_points / 3;
    
	  points = g_new0 (GimpVectorsCompatPoint, n_points);
    
	  for (i = 0, curr_point_pair = points_pairs;
	       i < n_points;
	       i++, curr_point_pair += 3)
	    {
	      points[i].x    = curr_point_pair[0];
	      points[i].y    = curr_point_pair[1];
	      points[i].type = curr_point_pair[2];
	    }
    
	  vectors = gimp_vectors_compat_new (gimage, name, points, n_points,
					     closed);
    
	  g_free (points);
    
	  if (vectors)
	    gimp_image_add_vectors (gimage, vectors, 0);
	  else
	    success = FALSE;
	}
    }

  return procedural_db_return_args (&path_set_points_proc, success);
}

static ProcArg path_set_points_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The ID of the image to set the paths in"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "The name of the path to create (if it exists then a unique name will be created - query the list of paths if you want to make sure that the name of the path you create is unique. This will be set as the current path."
  },
  {
    GIMP_PDB_INT32,
    "ptype",
    "The type of the path. Currently only one type (1 = Bezier) is supported"
  },
  {
    GIMP_PDB_INT32,
    "num_path_points",
    "The number of points in the path. Each point is made up of (x, y, type) of floats. Currently only the creation of bezier curves is allowed. The type parameter must be set to (1) to indicate a BEZIER type curve. For BEZIERS. Note the that points must be given in the following order... ACCACCAC ... If the path is not closed the last control point is missed off. Points consist of three control points (control/anchor/control) so for a curve that is not closed there must be at least two points passed (2 x,y pairs). If num_path_pnts % 3 = 0 then the path is assumed to be closed and the points are ACCACCACCACC."
  },
  {
    GIMP_PDB_FLOATARRAY,
    "points_pairs",
    "The points in the path represented as 3 floats. The first is the x pos, next is the y pos, last is the type of the pnt. The type field is dependant on the path type. For beziers (type 1 paths) the type can either be (1.0 = BEZIER_ANCHOR, 2.0 = BEZIER_CONTROL, 3.0= BEZIER_MOVE). Note all points are returned in pixel resolution"
  }
};

static ProcRecord path_set_points_proc =
{
  "gimp_path_set_points",
  "Set the points associated with the named path.",
  "Set the points associated with the named path.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  5,
  path_set_points_inargs,
  0,
  NULL,
  { { path_set_points_invoker } }
};

static Argument *
path_stroke_current_invoker (Gimp     *gimp,
                             Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  GimpVectors *vectors;
  GimpDrawable *drawable;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  if (success)
    {
      vectors  = gimp_image_get_active_vectors (gimage);
      drawable = gimp_image_active_drawable (gimage);
    
      if (vectors && drawable)
	{
	  GimpToolInfo *tool_info;
    
	  tool_info = gimp_context_get_tool (gimp_get_current_context (gimp));
    
	  success = gimp_item_stroke (GIMP_ITEM (vectors), drawable,
				      tool_info->paint_info);
	}
      else
	success = FALSE;
    }

  return procedural_db_return_args (&path_stroke_current_proc, success);
}

static ProcArg path_stroke_current_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The ID of the image which contains the path to stroke"
  }
};

static ProcRecord path_stroke_current_proc =
{
  "gimp_path_stroke_current",
  "Stroke the current path in the passed image.",
  "Stroke the current path in the passed image.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  1,
  path_stroke_current_inargs,
  0,
  NULL,
  { { path_stroke_current_invoker } }
};

static Argument *
path_get_point_at_dist_invoker (Gimp     *gimp,
                                Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gdouble distance;
  gint32 x_point = 0;
  gint32 y_point = 0;
  gdouble gradient = 0;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  distance = args[1].value.pdb_float;

  if (success)
    {
      vectors = gimp_image_get_active_vectors (gimage);
    
      if (vectors)
	{
	  g_warning ("FIXME: path_get_point_at_dist() is unimplemented");
	  success = FALSE;
	}
      else
	success = FALSE;
    }

  return_args = procedural_db_return_args (&path_get_point_at_dist_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = x_point;
      return_args[2].value.pdb_int = y_point;
      return_args[3].value.pdb_float = gradient;
    }

  return return_args;
}

static ProcArg path_get_point_at_dist_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The ID of the image the paths belongs to"
  },
  {
    GIMP_PDB_FLOAT,
    "distance",
    "The distance along the path"
  }
};

static ProcArg path_get_point_at_dist_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "x_point",
    "The x position of the point"
  },
  {
    GIMP_PDB_INT32,
    "y_point",
    "The y position of the point"
  },
  {
    GIMP_PDB_FLOAT,
    "gradient",
    "The gradient at the specified point"
  }
};

static ProcRecord path_get_point_at_dist_proc =
{
  "gimp_path_get_point_at_dist",
  "Get point on a path at a specified distance along the path.",
  "This will return the x,y position of a point at a given distance along the bezier curve. The distance will the obtained by first digitizing the curve internally an then walking along the curve. For a closed curve the start of the path is the first point on the path that was created. This might not be obvious. Note the current path is used.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  2,
  path_get_point_at_dist_inargs,
  3,
  path_get_point_at_dist_outargs,
  { { path_get_point_at_dist_invoker } }
};

static Argument *
path_get_tattoo_invoker (Gimp     *gimp,
                         Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gchar *name;
  gint32 tattoo = 0;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_name (gimage, name);
    
      if (vectors)
	tattoo = gimp_item_get_tattoo (GIMP_ITEM (vectors));
      else
	success = FALSE;
    }

  return_args = procedural_db_return_args (&path_get_tattoo_proc, success);

  if (success)
    return_args[1].value.pdb_int = tattoo;

  return return_args;
}

static ProcArg path_get_tattoo_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "the name of the path whose tattoo should be obtained"
  }
};

static ProcArg path_get_tattoo_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "tattoo",
    "The tattoo associated with the name path"
  }
};

static ProcRecord path_get_tattoo_proc =
{
  "gimp_path_get_tattoo",
  "Returns the tattoo associated with the name path.",
  "This procedure returns the tattoo associated with the specified path. A tattoo is a unique and permanent identifier attached to a path that can be used to uniquely identify a path within an image even between sessions.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  2,
  path_get_tattoo_inargs,
  1,
  path_get_tattoo_outargs,
  { { path_get_tattoo_invoker } }
};

static Argument *
path_set_tattoo_invoker (Gimp     *gimp,
                         Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  gchar *name;
  gint32 tattovalue = 0;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  tattovalue = args[2].value.pdb_int;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_name (gimage, name);
    
      if (vectors)
	gimp_item_set_tattoo (GIMP_ITEM (vectors), tattovalue);
      else
	success = FALSE;
    }

  return procedural_db_return_args (&path_set_tattoo_proc, success);
}

static ProcArg path_set_tattoo_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "the name of the path whose tattoo should be set"
  },
  {
    GIMP_PDB_INT32,
    "tattovalue",
    "The tattoo associated with the name path. Only values returned from 'path_get_tattoo' should be used here"
  }
};

static ProcRecord path_set_tattoo_proc =
{
  "gimp_path_set_tattoo",
  "Sets the tattoo associated with the name path.",
  "This procedure sets the tattoo associated with the specified path. A tattoo is a unique and permenant identifier attached to a path that can be used to uniquely identify a path within an image even between sessions. Note that the value passed to this function must have been obtained from a previous call to path_get_tattoo.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  3,
  path_set_tattoo_inargs,
  0,
  NULL,
  { { path_set_tattoo_invoker } }
};

static Argument *
get_path_by_tattoo_invoker (Gimp     *gimp,
                            Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gint32 tattoo;
  gchar *name = NULL;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  tattoo = args[1].value.pdb_int;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_tattoo (gimage, tattoo);
    
      if (vectors)
	name = g_strdup (gimp_object_get_name (GIMP_OBJECT (vectors)));
      else
	success = FALSE;
    }

  return_args = procedural_db_return_args (&get_path_by_tattoo_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = name;

  return return_args;
}

static ProcArg get_path_by_tattoo_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_INT32,
    "tattoo",
    "The tattoo of the required path"
  }
};

static ProcArg get_path_by_tattoo_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "name",
    "The name of the path with the specified tattoo"
  }
};

static ProcRecord get_path_by_tattoo_proc =
{
  "gimp_get_path_by_tattoo",
  "Return the name of the path with the given tattoo.",
  "The procedure returns the name of the path in the specified image which has the passed tattoo. The tattoos are unique within the image and will be preserved across sessions and through renaming of the path. An error is returned if no path woth the specified tattoo can be found.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  2,
  get_path_by_tattoo_inargs,
  1,
  get_path_by_tattoo_outargs,
  { { get_path_by_tattoo_invoker } }
};

static Argument *
path_get_locked_invoker (Gimp     *gimp,
                         Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gchar *name;
  gint32 lockstatus = 0;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_name (gimage, name);
    
      if (vectors)
	lockstatus = gimp_item_get_linked (GIMP_ITEM (vectors));
      else
	success = FALSE;
    }

  return_args = procedural_db_return_args (&path_get_locked_proc, success);

  if (success)
    return_args[1].value.pdb_int = lockstatus;

  return return_args;
}

static ProcArg path_get_locked_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "the name of the path whose locked status should be obtained"
  }
};

static ProcArg path_get_locked_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "lockstatus",
    "The lock status associated with the name path. 0 returned if the path is not locked. 1 is returned if the path is locked"
  }
};

static ProcRecord path_get_locked_proc =
{
  "gimp_path_get_locked",
  "Returns the locked status associated with the name path.",
  "This procedure returns the lock status associated with the specified path. A path can be \"locked\" which means that the transformation tool operations will also apply to the path.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  2,
  path_get_locked_inargs,
  1,
  path_get_locked_outargs,
  { { path_get_locked_invoker } }
};

static Argument *
path_set_locked_invoker (Gimp     *gimp,
                         Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  gchar *name;
  gint32 lockstatus = 0;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  lockstatus = args[2].value.pdb_int;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_name (gimage, name);
    
      if (vectors)
	gimp_item_set_linked (GIMP_ITEM (vectors), lockstatus, TRUE);
      else
	success = FALSE;
    }

  return procedural_db_return_args (&path_set_locked_proc, success);
}

static ProcArg path_set_locked_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "the name of the path whose locked status should be set"
  },
  {
    GIMP_PDB_INT32,
    "lockstatus",
    "The lock status associated with the name path. 0 if the path is not locked. 1 if the path is to be locked"
  }
};

static ProcRecord path_set_locked_proc =
{
  "gimp_path_set_locked",
  "Set the locked status associated with the name path.",
  "This procedure sets the lock status associated with the specified path. A path can be \"locked\" which means that the transformation tool operations will also apply to the path.",
  "Andy Thomas",
  "Andy Thomas",
  "1999",
  GIMP_INTERNAL,
  3,
  path_set_locked_inargs,
  0,
  NULL,
  { { path_set_locked_invoker } }
};

static Argument *
path_to_selection_invoker (Gimp     *gimp,
                           Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  gchar *name;
  gint32 op;
  gboolean antialias;
  gboolean feather;
  gdouble feather_radius_x;
  gdouble feather_radius_y;
  GimpVectors *vectors;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL || !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  op = args[2].value.pdb_int;
  if (op < GIMP_CHANNEL_OP_ADD || op > GIMP_CHANNEL_OP_INTERSECT)
    success = FALSE;

  antialias = args[3].value.pdb_int ? TRUE : FALSE;

  feather = args[4].value.pdb_int ? TRUE : FALSE;

  feather_radius_x = args[5].value.pdb_float;

  feather_radius_y = args[6].value.pdb_float;

  if (success)
    {
      vectors = gimp_image_get_vectors_by_name (gimage, name);
    
      if (vectors)
	gimp_image_mask_select_vectors (gimage,
					_("Path to Selection"),
					vectors,
					op,
					antialias,
					feather,
					feather_radius_x,
					feather_radius_y);
      else
	success = FALSE;
    }

  return procedural_db_return_args (&path_to_selection_proc, success);
}

static ProcArg path_to_selection_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "The name of the path which should be made into selection"
  },
  {
    GIMP_PDB_INT32,
    "op",
    "The desired operation with current selection"
  },
  {
    GIMP_PDB_INT32,
    "antialias",
    "Antialias selection"
  },
  {
    GIMP_PDB_INT32,
    "feather",
    "Feather selection"
  },
  {
    GIMP_PDB_FLOAT,
    "feather_radius_x",
    "Feather radius x"
  },
  {
    GIMP_PDB_FLOAT,
    "feather_radius_y",
    "Feather radius y"
  }
};

static ProcRecord path_to_selection_proc =
{
  "gimp_path_to_selection",
  "Transforms the active path into a selection",
  "This procedure renders the desired path into the current selection.",
  "Joao S. O. Bueno",
  "Joao S. O. Bueno",
  "2003",
  GIMP_INTERNAL,
  7,
  path_to_selection_inargs,
  0,
  NULL,
  { { path_to_selection_invoker } }
};
