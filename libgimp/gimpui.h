/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
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

#ifndef __GIMP_UI_H__
#define __GIMP_UI_H__

#include <gtk/gtk.h>

#include <libgimpwidgets/gimpwidgets.h>

#include <libgimp/gimpuitypes.h>

#include <libgimp/gimpexport.h>
#include <libgimp/gimpmenu.h>
#include <libgimp/gimpaspectpreview.h>
#include <libgimp/gimpbrushmenu.h>
#include <libgimp/gimpdrawablepreview.h>
#include <libgimp/gimpfontmenu.h>
#include <libgimp/gimpgradientmenu.h>
#include <libgimp/gimppalettemenu.h>
#include <libgimp/gimppatternmenu.h>
#include <libgimp/gimppixbuf.h>
#include <libgimp/gimpprocbrowserdialog.h>
#include <libgimp/gimpprocview.h>
#include <libgimp/gimpprogressbar.h>
#include <libgimp/gimpdrawablecombobox.h>
#include <libgimp/gimpimagecombobox.h>
#include <libgimp/gimpfontselectbutton.h>


G_BEGIN_DECLS

/* For information look into the C source or the html documentation */


void gimp_ui_init (const gchar *prog_name,
                   gboolean     preview);


G_END_DECLS

#endif /* __GIMP_UI_H__ */
