/* LIBGIMP - The GIMP Library 
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * parasite.c
 * Copyright (C) 1998 Jay Cox <jaycox@earthlink.net>
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
#include "gimp.h"

Parasite *
gimp_parasite_find (const char *name)
{
  GParam *return_vals;
  int nreturn_vals;
  Parasite *parasite;
  return_vals = gimp_run_procedure ("gimp_parasite_find",
				    &nreturn_vals,
				    PARAM_STRING, name,
				    PARAM_END);

  if (return_vals[0].data.d_status == STATUS_SUCCESS)
  {
    parasite = parasite_copy(&return_vals[1].data.d_parasite);
  }
  else
    parasite = NULL;

  gimp_destroy_params (return_vals, nreturn_vals);
  
  return parasite;
}


void
gimp_parasite_attach (const Parasite *p)
{
  GParam *return_vals;
  int nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_parasite_attach",
				    &nreturn_vals,
				    PARAM_PARASITE, p,
				    PARAM_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}

void
gimp_attach_new_parasite (const char *name, int flags,
			  int size, const void *data)
{
  GParam *return_vals;
  int nreturn_vals;
  Parasite *p = parasite_new(name, flags, size, data);

  return_vals = gimp_run_procedure ("gimp_parasite_attach",
				    &nreturn_vals,
				    PARAM_PARASITE, p,
				    PARAM_END);

  parasite_free(p);
  gimp_destroy_params (return_vals, nreturn_vals);
}

void
gimp_parasite_detach (const char *name)
{
  GParam *return_vals;
  int nreturn_vals;

  return_vals = gimp_run_procedure ("gimp_parasite_detach",
				    &nreturn_vals,
				    PARAM_STRING, name,
				    PARAM_END);

  gimp_destroy_params (return_vals, nreturn_vals);
}
