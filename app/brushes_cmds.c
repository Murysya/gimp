/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-1999 Spencer Kimball and Peter Mattis
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

#include "procedural_db.h"

#include <string.h>
#include "gimpbrush.h"
#include "gimpbrushlistP.h"
#include "gimplist.h"
#include "paint_funcs.h"
#include "paint_options.h"

static ProcRecord brushes_refresh_proc;
static ProcRecord brushes_get_brush_proc;
static ProcRecord brushes_set_brush_proc;
static ProcRecord brushes_get_opacity_proc;
static ProcRecord brushes_set_opacity_proc;
static ProcRecord brushes_get_spacing_proc;
static ProcRecord brushes_set_spacing_proc;
static ProcRecord brushes_get_paint_mode_proc;
static ProcRecord brushes_set_paint_mode_proc;
static ProcRecord brushes_list_proc;
static ProcRecord brushes_get_brush_data_proc;

void
register_brushes_procs (void)
{
  procedural_db_register (&brushes_refresh_proc);
  procedural_db_register (&brushes_get_brush_proc);
  procedural_db_register (&brushes_set_brush_proc);
  procedural_db_register (&brushes_get_opacity_proc);
  procedural_db_register (&brushes_set_opacity_proc);
  procedural_db_register (&brushes_get_spacing_proc);
  procedural_db_register (&brushes_set_spacing_proc);
  procedural_db_register (&brushes_get_paint_mode_proc);
  procedural_db_register (&brushes_set_paint_mode_proc);
  procedural_db_register (&brushes_list_proc);
  procedural_db_register (&brushes_get_brush_data_proc);
}

static Argument *
brushes_refresh_invoker (Argument *args)
{
  /* FIXME: I've hardcoded success to be 1, because brushes_init() is a 
   *        void function right now.  It'd be nice if it returned a value at 
   *        some future date, so we could tell if things blew up when reparsing
   *        the list (for whatever reason). 
   *                       - Seth "Yes, this is a kludge" Burgess
   *                         <sjburges@gimp.org>
   */

  brushes_init (FALSE);

  return procedural_db_return_args (&brushes_refresh_proc, TRUE);
}

static ProcRecord brushes_refresh_proc =
{
  "gimp_brushes_refresh",
  "Refresh current brushes.",
  "This procedure retrieves all brushes currently in the user's brush path and updates the brush dialog accordingly.",
  "Seth Burgess",
  "Seth Burgess",
  "1997",
  PDB_INTERNAL,
  0,
  NULL,
  0,
  NULL,
  { { brushes_refresh_invoker } }
};

static Argument *
brushes_get_brush_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpBrush *brushp;

  success = (brushp = get_active_brush ()) != NULL;

  return_args = procedural_db_return_args (&brushes_get_brush_proc, success);

  if (success)
    {
      return_args[1].value.pdb_pointer = g_strdup (brushp->name);
      return_args[2].value.pdb_int = brushp->mask->width;
      return_args[3].value.pdb_int = brushp->mask->height;
      return_args[4].value.pdb_int = brushp->spacing;
    }

  return return_args;
}

static ProcArg brushes_get_brush_outargs[] =
{
  {
    PDB_STRING,
    "name",
    "The brush name"
  },
  {
    PDB_INT32,
    "width",
    "The brush width"
  },
  {
    PDB_INT32,
    "height",
    "The brush height"
  },
  {
    PDB_INT32,
    "spacing",
    "The brush spacing: 0 <= spacing <= 1000"
  }
};

static ProcRecord brushes_get_brush_proc =
{
  "gimp_brushes_get_brush",
  "Retrieve information about the currently active brush mask.",
  "This procedure retrieves information about the currently active brush mask. This includes the brush name, the width and height, and the brush spacing paramter. All paint operations and stroke operations use this mask to control the application of paint to the image.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  0,
  NULL,
  4,
  brushes_get_brush_outargs,
  { { brushes_get_brush_invoker } }
};

static Argument *
brushes_set_brush_invoker (Argument *args)
{
  gboolean success = TRUE;
  gchar *name;
  GimpBrush *brushp;

  name = (gchar *) args[0].value.pdb_pointer;
  if (name == NULL)
    success = FALSE;

  if (success)
    {
      brushp = gimp_brush_list_get_brush (brush_list, name);
      if (brushp)
	select_brush (brushp);
      else
	success = FALSE;
    }

  return procedural_db_return_args (&brushes_set_brush_proc, success);
}

static ProcArg brushes_set_brush_inargs[] =
{
  {
    PDB_STRING,
    "name",
    "The brush name"
  }
};

static ProcRecord brushes_set_brush_proc =
{
  "gimp_brushes_set_brush",
  "Set the specified brush as the active brush.",
  "This procedure allows the active brush mask to be set by specifying its name. The name is simply a string which corresponds to one of the names of the installed brushes. If there is no matching brush found, this procedure will return an error. Otherwise, the specified brush becomes active and will be used in all subsequent paint operations.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  1,
  brushes_set_brush_inargs,
  0,
  NULL,
  { { brushes_set_brush_invoker } }
};

static Argument *
brushes_get_opacity_invoker (Argument *args)
{
  Argument *return_args;

  return_args = procedural_db_return_args (&brushes_get_opacity_proc, TRUE);
  return_args[1].value.pdb_float = paint_options_get_opacity () * 100.0;

  return return_args;
}

static ProcArg brushes_get_opacity_outargs[] =
{
  {
    PDB_FLOAT,
    "opacity",
    "The brush opacity: 0 <= opacity <= 100"
  }
};

static ProcRecord brushes_get_opacity_proc =
{
  "gimp_brushes_get_opacity",
  "Get the brush opacity.",
  "This procedure returns the opacity setting for brushes. This value is set globally and will remain the same even if the brush mask is changed. The return value is a floating point number between 0 and 100.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  0,
  NULL,
  1,
  brushes_get_opacity_outargs,
  { { brushes_get_opacity_invoker } }
};

static Argument *
brushes_set_opacity_invoker (Argument *args)
{
  gboolean success = TRUE;
  gdouble opacity;

  opacity = args[0].value.pdb_float;
  if (opacity < 0.0 || opacity > 100.0)
    success = FALSE;

  if (success)
    paint_options_set_opacity (opacity / 100.0);

  return procedural_db_return_args (&brushes_set_opacity_proc, success);
}

static ProcArg brushes_set_opacity_inargs[] =
{
  {
    PDB_FLOAT,
    "opacity",
    "The brush opacity: 0 <= opacity <= 100"
  }
};

static ProcRecord brushes_set_opacity_proc =
{
  "gimp_brushes_set_opacity",
  "Set the brush opacity.",
  "This procedure modifies the opacity setting for brushes. This value is set globally and will remain the same even if the brush mask is changed. The value should be a floating point number between 0 and 100.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  1,
  brushes_set_opacity_inargs,
  0,
  NULL,
  { { brushes_set_opacity_invoker } }
};

static Argument *
brushes_get_spacing_invoker (Argument *args)
{
  Argument *return_args;

  return_args = procedural_db_return_args (&brushes_get_spacing_proc, TRUE);
  return_args[1].value.pdb_int = gimp_brush_get_spacing (get_active_brush ());

  return return_args;
}

static ProcArg brushes_get_spacing_outargs[] =
{
  {
    PDB_INT32,
    "spacing",
    "The brush spacing: 0 <= spacing <= 1000"
  }
};

static ProcRecord brushes_get_spacing_proc =
{
  "gimp_brushes_get_spacing",
  "Get the brush spacing.",
  "This procedure returns the spacing setting for brushes. This value is set per brush and will change if a different brush is selected. The return value is an integer between 0 and 1000 which represents percentage of the maximum of the width and height of the mask.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  0,
  NULL,
  1,
  brushes_get_spacing_outargs,
  { { brushes_get_spacing_invoker } }
};

static Argument *
brushes_set_spacing_invoker (Argument *args)
{
  gboolean success = TRUE;
  gint32 spacing;

  spacing = args[0].value.pdb_int;
  if (spacing < 0 || spacing > 1000)
    success = FALSE;

  if (success)
    gimp_brush_set_spacing (get_active_brush(), spacing);

  return procedural_db_return_args (&brushes_set_spacing_proc, success);
}

static ProcArg brushes_set_spacing_inargs[] =
{
  {
    PDB_INT32,
    "spacing",
    "The brush spacing: 0 <= spacing <= 1000"
  }
};

static ProcRecord brushes_set_spacing_proc =
{
  "gimp_brushes_set_spacing",
  "Set the brush spacing.",
  "This procedure modifies the spacing setting for the current brush. This value is set on a per-brush basis and will change if a different brush mask is selected. The value should be a integer between 0 and 1000.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  1,
  brushes_set_spacing_inargs,
  0,
  NULL,
  { { brushes_set_spacing_invoker } }
};

static Argument *
brushes_get_paint_mode_invoker (Argument *args)
{
  Argument *return_args;

  return_args = procedural_db_return_args (&brushes_get_paint_mode_proc, TRUE);
  return_args[1].value.pdb_int = paint_options_get_paint_mode ();

  return return_args;
}

static ProcArg brushes_get_paint_mode_outargs[] =
{
  {
    PDB_INT32,
    "paint_mode",
    "The paint mode: { NORMAL (0), DISSOLVE (1), BEHIND (2), MULTIPLY/BURN (3), SCREEN (4), OVERLAY (5), DIFFERENCE (6), ADDITION (7), SUBTRACT (8), DARKEN_ONLY (9), LIGHTEN_ONLY (10), HUE (11), SATURATION (12), COLOR (13), VALUE (14), DIVIDE/DODGE (15) }"
  }
};

static ProcRecord brushes_get_paint_mode_proc =
{
  "gimp_brushes_get_paint_mode",
  "Get the brush paint mode.",
  "This procedure returns the paint-mode setting for brushes. This value is set globally and will not change if a different brush is selected. The return value is an integer which corresponds to the values listed in the argument description.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  0,
  NULL,
  1,
  brushes_get_paint_mode_outargs,
  { { brushes_get_paint_mode_invoker } }
};

static Argument *
brushes_set_paint_mode_invoker (Argument *args)
{
  gboolean success = TRUE;
  gint32 paint_mode;

  paint_mode = args[0].value.pdb_int;
  if (paint_mode < NORMAL_MODE || paint_mode > DIVIDE_MODE)
    success = FALSE;

  if (success)
    paint_options_set_paint_mode (paint_mode);

  return procedural_db_return_args (&brushes_set_paint_mode_proc, success);
}

static ProcArg brushes_set_paint_mode_inargs[] =
{
  {
    PDB_INT32,
    "paint_mode",
    "The paint mode: { NORMAL (0), DISSOLVE (1), BEHIND (2), MULTIPLY/BURN (3), SCREEN (4), OVERLAY (5), DIFFERENCE (6), ADDITION (7), SUBTRACT (8), DARKEN_ONLY (9), LIGHTEN_ONLY (10), HUE (11), SATURATION (12), COLOR (13), VALUE (14), DIVIDE/DODGE (15) }"
  }
};

static ProcRecord brushes_set_paint_mode_proc =
{
  "gimp_brushes_set_paint_mode",
  "Set the brush paint mode.",
  "This procedure modifies the paint_mode setting for the current brush. This value is set globally and will not change if a different brush mask is selected.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  1,
  brushes_set_paint_mode_inargs,
  0,
  NULL,
  { { brushes_set_paint_mode_invoker } }
};

static Argument *
brushes_list_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar **brushes;
  GSList *list = NULL;
  int i = 0;

  brushes = g_new (char *, brush_list->num_brushes);

  success = (list = GIMP_LIST (brush_list)->list) != NULL;

  while (list)
    {
      brushes[i++] = g_strdup (((GimpBrush *) list->data)->name);
      list = list->next;
    }

  return_args = procedural_db_return_args (&brushes_list_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = brush_list->num_brushes;
      return_args[2].value.pdb_pointer = brushes;
    }

  return return_args;
}

static ProcArg brushes_list_outargs[] =
{
  {
    PDB_INT32,
    "num_brushes",
    "The number of brushes in the brush list"
  },
  {
    PDB_STRINGARRAY,
    "brush_list",
    "The list of brush names"
  }
};

static ProcRecord brushes_list_proc =
{
  "gimp_brushes_list",
  "Retrieve a complete listing of the available brushes.",
  "This procedure returns a complete listing of available GIMP brushes. Each name returned can be used as input to the 'gimp_brushes_set_brush'.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  PDB_INTERNAL,
  0,
  NULL,
  2,
  brushes_list_outargs,
  { { brushes_list_invoker } }
};

static Argument *
brushes_get_brush_data_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *name;
  gint32 length = 0;
  gint8 *mask_data = NULL;
  GimpBrushP brushp = NULL;

  name = (gchar *) args[0].value.pdb_pointer;
  if (name == NULL)
    success = FALSE;

  if (success)
    {
      if (strlen (name))
	{
	  GSList *list = GIMP_LIST (brush_list)->list;
    
	  success = FALSE;
    
	  while (list)
	    {
	      brushp = (GimpBrushP) list->data;
    
	      if (!strcmp (brushp->name, name))
		{
		  success = TRUE;
		  break;
		}
    
	      list = list->next;
	    }
	}
      else
	success = (brushp = get_active_brush ()) != NULL;
    
      if (success)
	{
	  length = brushp->mask->height * brushp->mask->width;
	  mask_data = g_new (gint8, length);
	  g_memmove (mask_data, temp_buf_data (brushp->mask), length);
	}
    }

  return_args = procedural_db_return_args (&brushes_get_brush_data_proc, success);

  if (success)
    {
      return_args[1].value.pdb_pointer = g_strdup (brushp->name);
      return_args[2].value.pdb_float = 1.0;
      return_args[3].value.pdb_int = brushp->spacing;
      return_args[4].value.pdb_int = 0;
      return_args[5].value.pdb_int = brushp->mask->width;
      return_args[6].value.pdb_int = brushp->mask->height;
      return_args[7].value.pdb_int = length;
      return_args[8].value.pdb_pointer = mask_data;
    }

  return return_args;
}

static ProcArg brushes_get_brush_data_inargs[] =
{
  {
    PDB_STRING,
    "name",
    "the brush name (\"\" means current active pattern)"
  }
};

static ProcArg brushes_get_brush_data_outargs[] =
{
  {
    PDB_STRING,
    "name",
    "The brush name"
  },
  {
    PDB_FLOAT,
    "opacity",
    "The brush opacity: 0 <= opacity <= 100"
  },
  {
    PDB_INT32,
    "spacing",
    "The brush spacing: 0 <= spacing <= 1000"
  },
  {
    PDB_INT32,
    "paint_mode",
    "The paint mode: { NORMAL (0), DISSOLVE (1), BEHIND (2), MULTIPLY/BURN (3), SCREEN (4), OVERLAY (5), DIFFERENCE (6), ADDITION (7), SUBTRACT (8), DARKEN_ONLY (9), LIGHTEN_ONLY (10), HUE (11), SATURATION (12), COLOR (13), VALUE (14), DIVIDE/DODGE (15) }"
  },
  {
    PDB_INT32,
    "width",
    "The brush width"
  },
  {
    PDB_INT32,
    "height",
    "The brush height"
  },
  {
    PDB_INT32,
    "length",
    "Length of brush mask data"
  },
  {
    PDB_INT8ARRAY,
    "mask_data",
    "The brush mask data"
  }
};

static ProcRecord brushes_get_brush_data_proc =
{
  "gimp_brushes_get_brush_data",
  "Retrieve information about the currently active brush (including data).",
  "This procedure retrieves information about the currently active brush. This includes the brush name, and the brush extents (width and height). It also returns the brush data.",
  "Andy Thomas",
  "Andy Thomas",
  "1998",
  PDB_INTERNAL,
  1,
  brushes_get_brush_data_inargs,
  8,
  brushes_get_brush_data_outargs,
  { { brushes_get_brush_data_invoker } }
};
