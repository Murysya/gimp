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

#ifndef __GIMP_TEXT_OPTIONS_H__
#define __GIMP_TEXT_OPTIONS_H__


#include "tool_options.h"


#define GIMP_TYPE_TEXT_OPTIONS            (gimp_text_options_get_type ())
#define GIMP_TEXT_OPTIONS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_TEXT_OPTIONS, GimpTextOptions))
#define GIMP_TEXT_OPTIONS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_TEXT_OPTIONS, GimpTextOptionsClass))
#define GIMP_IS_TEXT_OPTIONS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_TEXT_OPTIONS))
#define GIMP_IS_TEXT_OPTIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_TEXT_OPTIONS))
#define GIMP_TEXT_OPTIONS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_TEXT_OPTIONS, GimpTextOptionsClass))


typedef struct _GimpTextOptions      GimpTextOptions;
typedef struct _GimpToolOptionsClass GimpTextOptionsClass;

struct _GimpTextOptions
{
  GimpToolOptions  tool_options;

  GimpText        *text;
  GtkTextBuffer   *buffer;
};


GType       gimp_text_options_get_type (void) G_GNUC_CONST;

GtkWidget * gimp_text_options_gui      (GimpToolOptions *tool_options);


#endif /* __GIMP_TEXT_OPTIONS_H__ */
