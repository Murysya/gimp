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

#include "libgimpbase/gimpbase.h"

#include "pdb-types.h"
#include "gimpargument.h"
#include "gimpprocedure.h"
#include "procedural_db.h"
#include "core/gimpparamspecs.h"

#include "core/gimp-parasites.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "vectors/gimpvectors.h"

static GimpProcedure parasite_find_proc;
static GimpProcedure parasite_attach_proc;
static GimpProcedure parasite_detach_proc;
static GimpProcedure parasite_list_proc;
static GimpProcedure image_parasite_find_proc;
static GimpProcedure image_parasite_attach_proc;
static GimpProcedure image_parasite_detach_proc;
static GimpProcedure image_parasite_list_proc;
static GimpProcedure drawable_parasite_find_proc;
static GimpProcedure drawable_parasite_attach_proc;
static GimpProcedure drawable_parasite_detach_proc;
static GimpProcedure drawable_parasite_list_proc;
static GimpProcedure vectors_parasite_find_proc;
static GimpProcedure vectors_parasite_attach_proc;
static GimpProcedure vectors_parasite_detach_proc;
static GimpProcedure vectors_parasite_list_proc;

void
register_parasite_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * parasite_find
   */
  procedure = gimp_procedure_init (&parasite_find_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The name of the parasite to find",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_PARASITE,
                                   gimp_param_spec_parasite ("parasite",
                                                             "parasite",
                                                             "The found parasite",
                                                             GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * parasite_attach
   */
  procedure = gimp_procedure_init (&parasite_attach_proc, 1, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_PARASITE,
                               gimp_param_spec_parasite ("parasite",
                                                         "parasite",
                                                         "The parasite to attach to the gimp",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * parasite_detach
   */
  procedure = gimp_procedure_init (&parasite_detach_proc, 1, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The name of the parasite to detach from the gimp.",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * parasite_list
   */
  procedure = gimp_procedure_init (&parasite_list_proc, 0, 2);
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-parasites",
                                                     "num parasites",
                                                     "The number of attached parasites",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("parasites",
                                                         "parasites",
                                                         "The names of currently attached parasites",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * image_parasite_find
   */
  procedure = gimp_procedure_init (&image_parasite_find_proc, 2, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_IMAGE,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         gimp,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The name of the parasite to find",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_PARASITE,
                                   gimp_param_spec_parasite ("parasite",
                                                             "parasite",
                                                             "The found parasite",
                                                             GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * image_parasite_attach
   */
  procedure = gimp_procedure_init (&image_parasite_attach_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_IMAGE,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         gimp,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_PARASITE,
                               gimp_param_spec_parasite ("parasite",
                                                         "parasite",
                                                         "The parasite to attach to an image",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * image_parasite_detach
   */
  procedure = gimp_procedure_init (&image_parasite_detach_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_IMAGE,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         gimp,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The name of the parasite to detach from an image.",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * image_parasite_list
   */
  procedure = gimp_procedure_init (&image_parasite_list_proc, 1, 2);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_IMAGE,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         gimp,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-parasites",
                                                     "num parasites",
                                                     "The number of attached parasites",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("parasites",
                                                         "parasites",
                                                         "The names of currently attached parasites",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * drawable_parasite_find
   */
  procedure = gimp_procedure_init (&drawable_parasite_find_proc, 2, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The name of the parasite to find",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_PARASITE,
                                   gimp_param_spec_parasite ("parasite",
                                                             "parasite",
                                                             "The found parasite",
                                                             GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * drawable_parasite_attach
   */
  procedure = gimp_procedure_init (&drawable_parasite_attach_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_PARASITE,
                               gimp_param_spec_parasite ("parasite",
                                                         "parasite",
                                                         "The parasite to attach to a drawable",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * drawable_parasite_detach
   */
  procedure = gimp_procedure_init (&drawable_parasite_detach_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The name of the parasite to detach from a drawable.",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * drawable_parasite_list
   */
  procedure = gimp_procedure_init (&drawable_parasite_list_proc, 1, 2);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_DRAWABLE,
                               gimp_param_spec_item_id ("drawable",
                                                        "drawable",
                                                        "The drawable",
                                                        gimp,
                                                        GIMP_TYPE_DRAWABLE,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-parasites",
                                                     "num parasites",
                                                     "The number of attached parasites",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("parasites",
                                                         "parasites",
                                                         "The names of currently attached parasites",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * vectors_parasite_find
   */
  procedure = gimp_procedure_init (&vectors_parasite_find_proc, 2, 1);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_VECTORS,
                               gimp_param_spec_item_id ("vectors",
                                                        "vectors",
                                                        "The vectors object",
                                                        gimp,
                                                        GIMP_TYPE_VECTORS,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The name of the parasite to find",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_PARASITE,
                                   gimp_param_spec_parasite ("parasite",
                                                             "parasite",
                                                             "The found parasite",
                                                             GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * vectors_parasite_attach
   */
  procedure = gimp_procedure_init (&vectors_parasite_attach_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_VECTORS,
                               gimp_param_spec_item_id ("vectors",
                                                        "vectors",
                                                        "The vectors object",
                                                        gimp,
                                                        GIMP_TYPE_VECTORS,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_PARASITE,
                               gimp_param_spec_parasite ("parasite",
                                                         "parasite",
                                                         "The parasite to attach to a vectors object",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * vectors_parasite_detach
   */
  procedure = gimp_procedure_init (&vectors_parasite_detach_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_VECTORS,
                               gimp_param_spec_item_id ("vectors",
                                                        "vectors",
                                                        "The vectors object",
                                                        gimp,
                                                        GIMP_TYPE_VECTORS,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_STRING,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The name of the parasite to detach from a vectors object.",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * vectors_parasite_list
   */
  procedure = gimp_procedure_init (&vectors_parasite_list_proc, 1, 2);
  gimp_procedure_add_argument (procedure,
                               GIMP_PDB_VECTORS,
                               gimp_param_spec_item_id ("vectors",
                                                        "vectors",
                                                        "The vectors object",
                                                        gimp,
                                                        GIMP_TYPE_VECTORS,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_INT32,
                                   g_param_spec_int ("num-parasites",
                                                     "num parasites",
                                                     "The number of attached parasites",
                                                     0, G_MAXINT32, 0,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   GIMP_PDB_STRINGARRAY,
                                   g_param_spec_pointer ("parasites",
                                                         "parasites",
                                                         "The names of currently attached parasites",
                                                         GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static GimpArgument *
parasite_find_invoker (GimpProcedure *procedure,
                       Gimp          *gimp,
                       GimpContext   *context,
                       GimpProgress  *progress,
                       GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  gchar *name;
  GimpParasite *parasite = NULL;

  name = (gchar *) g_value_get_string (&args[0].value);

  if (success)
    {
      parasite = gimp_parasite_copy (gimp_parasite_find (gimp, name));

      if (! parasite)
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_boxed (&return_vals[1].value, parasite);

  return return_vals;
}

static GimpProcedure parasite_find_proc =
{
  TRUE, TRUE,
  "gimp-parasite-find",
  "gimp-parasite-find",
  "Finds the named parasite.",
  "Finds and returns the named parasite that was previously attached to the gimp.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { parasite_find_invoker } }
};

static GimpArgument *
parasite_attach_invoker (GimpProcedure *procedure,
                         Gimp          *gimp,
                         GimpContext   *context,
                         GimpProgress  *progress,
                         GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpParasite *parasite;

  parasite = g_value_get_boxed (&args[0].value);

  if (success)
    {
      gimp_parasite_attach (gimp, parasite);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure parasite_attach_proc =
{
  TRUE, TRUE,
  "gimp-parasite-attach",
  "gimp-parasite-attach",
  "Add a parasite to the gimp.",
  "This procedure attaches a parasite to the gimp. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { parasite_attach_invoker } }
};

static GimpArgument *
parasite_detach_invoker (GimpProcedure *procedure,
                         Gimp          *gimp,
                         GimpContext   *context,
                         GimpProgress  *progress,
                         GimpArgument  *args)
{
  gboolean success = TRUE;
  gchar *name;

  name = (gchar *) g_value_get_string (&args[0].value);

  if (success)
    {
      gimp_parasite_detach (gimp, name);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure parasite_detach_proc =
{
  TRUE, TRUE,
  "gimp-parasite-detach",
  "gimp-parasite-detach",
  "Removes a parasite from the gimp.",
  "This procedure detaches a parasite from the gimp. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { parasite_detach_invoker } }
};

static GimpArgument *
parasite_list_invoker (GimpProcedure *procedure,
                       Gimp          *gimp,
                       GimpContext   *context,
                       GimpProgress  *progress,
                       GimpArgument  *args)
{
  GimpArgument *return_vals;
  gint32 num_parasites = 0;
  gchar **parasites = NULL;

  parasites = gimp_parasite_list (gimp, &num_parasites);

  return_vals = gimp_procedure_get_return_values (procedure, TRUE);

  g_value_set_int (&return_vals[1].value, num_parasites);
  g_value_set_pointer (&return_vals[2].value, parasites);

  return return_vals;
}

static GimpProcedure parasite_list_proc =
{
  TRUE, TRUE,
  "gimp-parasite-list",
  "gimp-parasite-list",
  "List all parasites.",
  "Returns a list of all currently attached parasites.",
  "Marc Lehmann",
  "Marc Lehmann",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { parasite_list_invoker } }
};

static GimpArgument *
image_parasite_find_invoker (GimpProcedure *procedure,
                             Gimp          *gimp,
                             GimpContext   *context,
                             GimpProgress  *progress,
                             GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  GimpImage *image;
  gchar *name;
  GimpParasite *parasite = NULL;

  image = gimp_value_get_image (&args[0].value, gimp);
  name = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      parasite = gimp_parasite_copy (gimp_image_parasite_find (image, name));

      if (! parasite)
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_boxed (&return_vals[1].value, parasite);

  return return_vals;
}

static GimpProcedure image_parasite_find_proc =
{
  TRUE, TRUE,
  "gimp-image-parasite-find",
  "gimp-image-parasite-find",
  "Finds the named parasite in an image",
  "Finds and returns the named parasite that was previously attached to an image.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { image_parasite_find_invoker } }
};

static GimpArgument *
image_parasite_attach_invoker (GimpProcedure *procedure,
                               Gimp          *gimp,
                               GimpContext   *context,
                               GimpProgress  *progress,
                               GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  GimpParasite *parasite;

  image = gimp_value_get_image (&args[0].value, gimp);
  parasite = g_value_get_boxed (&args[1].value);

  if (success)
    {
      gimp_image_parasite_attach (image, parasite);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure image_parasite_attach_proc =
{
  TRUE, TRUE,
  "gimp-image-parasite-attach",
  "gimp-image-parasite-attach",
  "Add a parasite to an image.",
  "This procedure attaches a parasite to an image. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { image_parasite_attach_invoker } }
};

static GimpArgument *
image_parasite_detach_invoker (GimpProcedure *procedure,
                               Gimp          *gimp,
                               GimpContext   *context,
                               GimpProgress  *progress,
                               GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gchar *name;

  image = gimp_value_get_image (&args[0].value, gimp);
  name = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      gimp_image_parasite_detach (image, name);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure image_parasite_detach_proc =
{
  TRUE, TRUE,
  "gimp-image-parasite-detach",
  "gimp-image-parasite-detach",
  "Removes a parasite from an image.",
  "This procedure detaches a parasite from an image. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { image_parasite_detach_invoker } }
};

static GimpArgument *
image_parasite_list_invoker (GimpProcedure *procedure,
                             Gimp          *gimp,
                             GimpContext   *context,
                             GimpProgress  *progress,
                             GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  GimpImage *image;
  gint32 num_parasites = 0;
  gchar **parasites = NULL;

  image = gimp_value_get_image (&args[0].value, gimp);

  if (success)
    {
      parasites = gimp_image_parasite_list (image, &num_parasites);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, num_parasites);
      g_value_set_pointer (&return_vals[2].value, parasites);
    }

  return return_vals;
}

static GimpProcedure image_parasite_list_proc =
{
  TRUE, TRUE,
  "gimp-image-parasite-list",
  "gimp-image-parasite-list",
  "List all parasites.",
  "Returns a list of all currently attached parasites.",
  "Marc Lehmann",
  "Marc Lehmann",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { image_parasite_list_invoker } }
};

static GimpArgument *
drawable_parasite_find_invoker (GimpProcedure *procedure,
                                Gimp          *gimp,
                                GimpContext   *context,
                                GimpProgress  *progress,
                                GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  GimpDrawable *drawable;
  gchar *name;
  GimpParasite *parasite = NULL;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  name = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      parasite = gimp_parasite_copy (gimp_item_parasite_find (GIMP_ITEM (drawable),
                                                              name));

      if (! parasite)
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_boxed (&return_vals[1].value, parasite);

  return return_vals;
}

static GimpProcedure drawable_parasite_find_proc =
{
  TRUE, TRUE,
  "gimp-drawable-parasite-find",
  "gimp-drawable-parasite-find",
  "Finds the named parasite in a drawable",
  "Finds and returns the named parasite that was previously attached to a drawable.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { drawable_parasite_find_invoker } }
};

static GimpArgument *
drawable_parasite_attach_invoker (GimpProcedure *procedure,
                                  Gimp          *gimp,
                                  GimpContext   *context,
                                  GimpProgress  *progress,
                                  GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpDrawable *drawable;
  GimpParasite *parasite;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  parasite = g_value_get_boxed (&args[1].value);

  if (success)
    {
      gimp_item_parasite_attach (GIMP_ITEM (drawable), parasite);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure drawable_parasite_attach_proc =
{
  TRUE, TRUE,
  "gimp-drawable-parasite-attach",
  "gimp-drawable-parasite-attach",
  "Add a parasite to a drawable.",
  "This procedure attaches a parasite to a drawable. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { drawable_parasite_attach_invoker } }
};

static GimpArgument *
drawable_parasite_detach_invoker (GimpProcedure *procedure,
                                  Gimp          *gimp,
                                  GimpContext   *context,
                                  GimpProgress  *progress,
                                  GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpDrawable *drawable;
  gchar *name;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  name = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      gimp_item_parasite_detach (GIMP_ITEM (drawable), name);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure drawable_parasite_detach_proc =
{
  TRUE, TRUE,
  "gimp-drawable-parasite-detach",
  "gimp-drawable-parasite-detach",
  "Removes a parasite from a drawable.",
  "This procedure detaches a parasite from a drawable. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { drawable_parasite_detach_invoker } }
};

static GimpArgument *
drawable_parasite_list_invoker (GimpProcedure *procedure,
                                Gimp          *gimp,
                                GimpContext   *context,
                                GimpProgress  *progress,
                                GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  GimpDrawable *drawable;
  gint32 num_parasites = 0;
  gchar **parasites = NULL;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);

  if (success)
    {
      parasites = gimp_item_parasite_list (GIMP_ITEM (drawable), &num_parasites);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, num_parasites);
      g_value_set_pointer (&return_vals[2].value, parasites);
    }

  return return_vals;
}

static GimpProcedure drawable_parasite_list_proc =
{
  TRUE, TRUE,
  "gimp-drawable-parasite-list",
  "gimp-drawable-parasite-list",
  "List all parasites.",
  "Returns a list of all currently attached parasites.",
  "Marc Lehmann",
  "Marc Lehmann",
  "1999",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { drawable_parasite_list_invoker } }
};

static GimpArgument *
vectors_parasite_find_invoker (GimpProcedure *procedure,
                               Gimp          *gimp,
                               GimpContext   *context,
                               GimpProgress  *progress,
                               GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  GimpVectors *vectors;
  gchar *name;
  GimpParasite *parasite = NULL;

  vectors = (GimpVectors *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_VECTORS);
  name = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      parasite = gimp_parasite_copy (gimp_item_parasite_find (GIMP_ITEM (vectors),
                                                              name));

      if (! parasite)
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_take_boxed (&return_vals[1].value, parasite);

  return return_vals;
}

static GimpProcedure vectors_parasite_find_proc =
{
  TRUE, TRUE,
  "gimp-vectors-parasite-find",
  "gimp-vectors-parasite-find",
  "Finds the named parasite in a vectors object",
  "Finds and returns the named parasite that was previously attached to a vectors object.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2006",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { vectors_parasite_find_invoker } }
};

static GimpArgument *
vectors_parasite_attach_invoker (GimpProcedure *procedure,
                                 Gimp          *gimp,
                                 GimpContext   *context,
                                 GimpProgress  *progress,
                                 GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpVectors *vectors;
  GimpParasite *parasite;

  vectors = (GimpVectors *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_VECTORS);
  parasite = g_value_get_boxed (&args[1].value);

  if (success)
    {
      gimp_item_parasite_attach (GIMP_ITEM (vectors), parasite);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure vectors_parasite_attach_proc =
{
  TRUE, TRUE,
  "gimp-vectors-parasite-attach",
  "gimp-vectors-parasite-attach",
  "Add a parasite to a vectors object",
  "This procedure attaches a parasite to a vectors object. It has no return values.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2006",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { vectors_parasite_attach_invoker } }
};

static GimpArgument *
vectors_parasite_detach_invoker (GimpProcedure *procedure,
                                 Gimp          *gimp,
                                 GimpContext   *context,
                                 GimpProgress  *progress,
                                 GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpVectors *vectors;
  gchar *name;

  vectors = (GimpVectors *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_VECTORS);
  name = (gchar *) g_value_get_string (&args[1].value);

  if (success)
    {
      gimp_item_parasite_detach (GIMP_ITEM (vectors), name);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure vectors_parasite_detach_proc =
{
  TRUE, TRUE,
  "gimp-vectors-parasite-detach",
  "gimp-vectors-parasite-detach",
  "Removes a parasite from a vectors object",
  "This procedure detaches a parasite from a vectors object. It has no return values.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2006",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { vectors_parasite_detach_invoker } }
};

static GimpArgument *
vectors_parasite_list_invoker (GimpProcedure *procedure,
                               Gimp          *gimp,
                               GimpContext   *context,
                               GimpProgress  *progress,
                               GimpArgument  *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  GimpVectors *vectors;
  gint32 num_parasites = 0;
  gchar **parasites = NULL;

  vectors = (GimpVectors *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_VECTORS);

  if (success)
    {
      parasites = gimp_item_parasite_list (GIMP_ITEM (vectors), &num_parasites);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, num_parasites);
      g_value_set_pointer (&return_vals[2].value, parasites);
    }

  return return_vals;
}

static GimpProcedure vectors_parasite_list_proc =
{
  TRUE, TRUE,
  "gimp-vectors-parasite-list",
  "gimp-vectors-parasite-list",
  "List all parasites.",
  "Returns a list of all currently attached parasites.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2006",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { vectors_parasite_list_invoker } }
};
