/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2000 Peter Mattis and Spencer Kimball
 *
 * gimpchannel_pdb.c
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

#include "gimp.h"

gint32
_gimp_channel_new (gint32   image_ID,
		   gint     width,
		   gint     height,
		   gchar   *name,
		   gdouble  opacity,
		   guchar   red,
		   guchar   green,
		   guchar   blue)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint32 channel_ID = -1;
  guchar color[3];

  color[0] = red;
  color[1] = green;
  color[2] = blue;

  return_vals = gimp_run_procedure ("gimp_channel_new",
				    &nreturn_vals,
				    GIMP_PDB_IMAGE, image_ID,
				    GIMP_PDB_INT32, width,
				    GIMP_PDB_INT32, height,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_FLOAT, opacity,
				    GIMP_PDB_COLOR, color,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    channel_ID = return_vals[1].data.d_channel;

  gimp_destroy_params (return_vals, nreturn_vals);

  return channel_ID;
}

gint32
gimp_channel_copy (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint32 channel_copy_ID = -1;

  return_vals = gimp_run_procedure ("gimp_channel_copy",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    channel_copy_ID = return_vals[1].data.d_channel;

  gimp_destroy_params (return_vals, nreturn_vals);

  return channel_copy_ID;
}

void
gimp_channel_delete (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_channel_delete",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}

gchar *
gimp_channel_get_name (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *name = NULL;

  return_vals = gimp_run_procedure ("gimp_channel_get_name",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    name = g_strdup (return_vals[1].data.d_string);

  gimp_destroy_params (return_vals, nreturn_vals);

  return name;
}

void
gimp_channel_set_name (gint32  channel_ID,
		       gchar  *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_channel_set_name",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}

gboolean
gimp_channel_get_visible (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean visible = FALSE;

  return_vals = gimp_run_procedure ("gimp_channel_get_visible",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    visible = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return visible;
}

void
gimp_channel_set_visible (gint32   channel_ID,
			  gboolean visible)
{
  GimpParam *return_vals;
  gint nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_channel_set_visible",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_INT32, visible,
				    GIMP_PDB_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}

gboolean
gimp_channel_get_show_masked (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean show_masked = FALSE;

  return_vals = gimp_run_procedure ("gimp_channel_get_show_masked",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    show_masked = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return show_masked;
}

void
gimp_channel_set_show_masked (gint32   channel_ID,
			      gboolean show_masked)
{
  GimpParam *return_vals;
  gint nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_channel_set_show_masked",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_INT32, show_masked,
				    GIMP_PDB_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}

gdouble
gimp_channel_get_opacity (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gdouble opacity = 0;

  return_vals = gimp_run_procedure ("gimp_channel_get_opacity",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    opacity = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return opacity;
}

void
gimp_channel_set_opacity (gint32  channel_ID,
			  gdouble opacity)
{
  GimpParam *return_vals;
  gint nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_channel_set_opacity",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_FLOAT, opacity,
				    GIMP_PDB_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}

void
gimp_channel_get_color (gint32  channel_ID,
			guchar *red,
			guchar *green,
			guchar *blue)
{
  GimpParam *return_vals;
  gint nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_channel_get_color",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    {
      *red = return_vals[1].data.d_color.red;
      *green = return_vals[1].data.d_color.green;
      *blue = return_vals[1].data.d_color.blue;
    }

  gimp_destroy_params (return_vals, nreturn_vals);
}

void
gimp_channel_set_color (gint32 channel_ID,
			guchar red,
			guchar green,
			guchar blue)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  guchar color[3];

  color[0] = red;
  color[1] = green;
  color[2] = blue;

  return_vals = gimp_run_procedure ("gimp_channel_set_color",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_COLOR, color,
				    GIMP_PDB_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}

gint
gimp_channel_get_tattoo (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint tattoo = 0;

  return_vals = gimp_run_procedure ("gimp_channel_get_tattoo",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    tattoo = return_vals[1].data.d_tattoo;

  gimp_destroy_params (return_vals, nreturn_vals);

  return tattoo;
}

void
gimp_channel_set_tattoo (gint32 channel_ID,
			 gint   tattoo)
{
  GimpParam *return_vals;
  gint nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_channel_set_tattoo",
				    &nreturn_vals,
				    GIMP_PDB_CHANNEL, channel_ID,
				    GIMP_PDB_INT32, tattoo,
				    GIMP_PDB_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}
