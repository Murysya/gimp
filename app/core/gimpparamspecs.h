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

#ifndef __GIMP_PARAM_SPECS_H__
#define __GIMP_PARAM_SPECS_H__


/*
 * Keep in sync with libgimpconfig/gimpconfig-params.h
 */
#define GIMP_PARAM_NO_VALIDATE (1 << (6 + G_PARAM_USER_SHIFT))


/*
 * GIMP_TYPE_PARAM_STRING
 */

#define GIMP_TYPE_PARAM_STRING           (gimp_param_string_get_type ())
#define GIMP_PARAM_SPEC_STRING(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_STRING, GimpParamSpecString))
#define GIMP_IS_PARAM_SPEC_STRING(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_STRING))

typedef struct _GimpParamSpecString GimpParamSpecString;

struct _GimpParamSpecString
{
  GParamSpecString parent_instance;

  guint            no_validate : 1;
  guint            null_ok     : 1;
};

GType        gimp_param_string_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_string      (const gchar  *name,
                                          const gchar  *nick,
                                          const gchar  *blurb,
                                          gboolean      no_validate,
                                          gboolean      null_ok,
                                          const gchar  *default_value,
                                          GParamFlags   flags);


/*
 * GIMP_TYPE_PARAM_ENUM
 */

#define GIMP_TYPE_PARAM_ENUM           (gimp_param_enum_get_type ())
#define GIMP_PARAM_SPEC_ENUM(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_ENUM, GimpParamSpecEnum))

#define GIMP_IS_PARAM_SPEC_ENUM(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_ENUM))

typedef struct _GimpParamSpecEnum GimpParamSpecEnum;

struct _GimpParamSpecEnum
{
  GParamSpecEnum  parent_instance;

  GSList         *excluded_values;
};

GType        gimp_param_enum_get_type     (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_enum         (const gchar       *name,
                                           const gchar       *nick,
                                           const gchar       *blurb,
                                           GType              enum_type,
                                           gint               default_value,
                                           GParamFlags        flags);

void   gimp_param_spec_enum_exclude_value (GimpParamSpecEnum *espec,
                                           gint               value);


/*
 * GIMP_TYPE_PARAM_IMAGE_ID
 */

#define GIMP_TYPE_PARAM_IMAGE_ID           (gimp_param_image_id_get_type ())
#define GIMP_PARAM_SPEC_IMAGE_ID(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_IMAGE_ID, GimpParamSpecImageID))
#define GIMP_IS_PARAM_SPEC_IMAGE_ID(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_IMAGE_ID))

typedef struct _GimpParamSpecImageID GimpParamSpecImageID;

struct _GimpParamSpecImageID
{
  GParamSpecInt  parent_instance;

  Gimp          *gimp;
};

GType        gimp_param_image_id_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_image_id      (const gchar  *name,
                                            const gchar  *nick,
                                            const gchar  *blurb,
                                            Gimp         *gimp,
                                            GParamFlags   flags);

GimpImage  * gimp_value_get_image          (const GValue *value,
                                            Gimp         *gimp);
void         gimp_value_set_image          (GValue       *value,
                                            GimpImage    *image);



/*
 * GIMP_TYPE_PARAM_ITEM_ID
 */

#define GIMP_TYPE_PARAM_ITEM_ID           (gimp_param_item_id_get_type ())
#define GIMP_PARAM_SPEC_ITEM_ID(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_ITEM_ID, GimpParamSpecItemID))
#define GIMP_IS_PARAM_SPEC_ITEM_ID(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_ITEM_ID))

typedef struct _GimpParamSpecItemID GimpParamSpecItemID;

struct _GimpParamSpecItemID
{
  GParamSpecInt  parent_instance;

  Gimp          *gimp;
  GType          item_type;
};

GType        gimp_param_item_id_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_item_id      (const gchar  *name,
                                           const gchar  *nick,
                                           const gchar  *blurb,
                                           Gimp         *gimp,
                                           GType         item_type,
                                           GParamFlags   flags);

GimpItem   * gimp_value_get_item          (const GValue *value,
                                           Gimp         *gimp,
                                           GType         item_type);
void         gimp_value_set_item          (GValue       *value,
                                           GimpItem     *item);


/*
 * GIMP_TYPE_PARAM_DISPLAY_ID
 */

#define GIMP_TYPE_PARAM_DISPLAY_ID           (gimp_param_display_id_get_type ())
#define GIMP_PARAM_SPEC_DISPLAY_ID(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_DISPLAY_ID, GimpParamSpecDisplayID))
#define GIMP_IS_PARAM_SPEC_DISPLAY_ID(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_DISPLAY_ID))

typedef struct _GimpParamSpecDisplayID GimpParamSpecDisplayID;

struct _GimpParamSpecDisplayID
{
  GParamSpecInt  parent_instance;

  Gimp          *gimp;
};

GType        gimp_param_display_id_get_type  (void) G_GNUC_CONST;

GParamSpec * gimp_param_spec_display_id      (const gchar  *name,
                                              const gchar  *nick,
                                              const gchar  *blurb,
                                              Gimp         *gimp,
                                              GParamFlags   flags);

GimpObject * gimp_value_get_display          (const GValue *value,
                                              Gimp         *gimp);
void         gimp_value_set_display          (GValue       *value,
                                              GimpObject   *display);


/*
 * GIMP_TYPE_ARRAY
 */

typedef struct _GimpArray GimpArray;

struct _GimpArray
{
  guint8   *data;
  gsize     length;
  gboolean  static_data;
};

GimpArray * gimp_array_new (const guint8     *data,
                            gsize             length,
                            gboolean          static_data);
GimpArray * gimp_array_copy (const GimpArray *array);
void        gimp_array_free (GimpArray       *array);

#define GIMP_TYPE_ARRAY               (gimp_array_get_type ())
#define GIMP_VALUE_HOLDS_ARRAY(value) (G_TYPE_CHECK_VALUE_TYPE ((value), GIMP_TYPE_ARRAY))

GType   gimp_array_get_type           (void) G_GNUC_CONST;


/*
 * GIMP_TYPE_PARAM_ARRAY
 */

#define GIMP_TYPE_PARAM_ARRAY           (gimp_param_array_get_type ())
#define GIMP_PARAM_SPEC_ARRAY(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_ARRAY, GimpParamSpecArray))
#define GIMP_IS_PARAM_SPEC_ARRAY(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_ARRAY))

typedef struct _GimpParamSpecArray GimpParamSpecArray;

struct _GimpParamSpecArray
{
  GParamSpecBoxed parent_instance;
};

GType           gimp_param_array_get_type         (void) G_GNUC_CONST;

GParamSpec    * gimp_param_spec_array             (const gchar  *name,
                                                   const gchar  *nick,
                                                   const gchar  *blurb,
                                                   GParamFlags   flags);

const guint8  * gimp_value_get_int8array          (const GValue *value);
guint8        * gimp_value_dup_int8array          (const GValue *value);
void            gimp_value_set_int8array          (GValue       *value,
                                                   const guint8 *array,
                                                   gsize         length);
void            gimp_value_set_static_int8array   (GValue       *value,
                                                   const guint8 *array,
                                                   gsize         length);
void            gimp_value_take_int8array         (GValue       *value,
                                                   guint8       *array,
                                                   gsize         length);

const gint16  * gimp_value_get_int16array         (const GValue *value);
gint16        * gimp_value_dup_int16array         (const GValue *value);
void            gimp_value_set_int16array         (GValue       *value,
                                                   const gint16 *array,
                                                   gsize         length);
void            gimp_value_set_static_int16array  (GValue       *value,
                                                   const gint16 *array,
                                                   gsize         length);
void            gimp_value_take_int16array        (GValue       *value,
                                                   gint16       *array,
                                                   gsize         length);

const gint32  * gimp_value_get_int32array         (const GValue *value);
gint32        * gimp_value_dup_int32array         (const GValue *value);
void            gimp_value_set_int32array         (GValue       *value,
                                                   const gint32 *array,
                                                   gsize         length);
void            gimp_value_set_static_int32array  (GValue       *value,
                                                   const gint32 *array,
                                                   gsize         length);
void            gimp_value_take_int32array        (GValue       *value,
                                                   gint32       *array,
                                                   gsize         length);

const gdouble * gimp_value_get_floatarray         (const GValue  *value);
gdouble       * gimp_value_dup_floatarray         (const GValue  *value);
void            gimp_value_set_floatarray         (GValue        *value,
                                                   const gdouble *array,
                                                   gsize         length);
void            gimp_value_set_static_floatarray  (GValue        *value,
                                                   const gdouble *array,
                                                   gsize         length);
void            gimp_value_take_floatarray        (GValue        *value,
                                                   gdouble       *array,
                                                   gsize         length);


/*
 * GIMP_TYPE_STRING_ARRAY
 */

GimpArray * gimp_string_array_new  (const gchar     **data,
                                    gsize             length,
                                    gboolean          static_data);
GimpArray * gimp_string_array_copy (const GimpArray  *array);
void        gimp_string_array_free (GimpArray        *array);

#define GIMP_TYPE_STRING_ARRAY               (gimp_string_array_get_type ())
#define GIMP_VALUE_HOLDS_STRING_ARRAY(value) (G_TYPE_CHECK_VALUE_TYPE ((value), GIMP_TYPE_STRING_ARRAY))

GType   gimp_string_array_get_type           (void) G_GNUC_CONST;


/*
 * GIMP_TYPE_PARAM_STRING_ARRAY
 */

#define GIMP_TYPE_PARAM_STRING_ARRAY           (gimp_param_string_array_get_type ())
#define GIMP_PARAM_SPEC_STRING_ARRAY(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_STRING_ARRAY, GimpParamSpecStringArray))
#define GIMP_IS_PARAM_SPEC_STRING_ARRAY(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GIMP_TYPE_PARAM_STRING_ARRAY))

typedef struct _GimpParamSpecStringArray GimpParamSpecStringArray;

struct _GimpParamSpecStringArray
{
  GParamSpecBoxed parent_instance;
};

GType          gimp_param_string_array_get_type  (void) G_GNUC_CONST;

GParamSpec   * gimp_param_spec_string_array      (const gchar  *name,
                                                  const gchar  *nick,
                                                  const gchar  *blurb,
                                                  GParamFlags   flags);

const gchar ** gimp_value_get_stringarray        (const GValue *value);
gchar       ** gimp_value_dup_stringarray        (const GValue *value);
void           gimp_value_set_stringarray        (GValue       *value,
                                                  const gchar **array,
                                                  gsize         length);
void           gimp_value_set_static_stringarray (GValue       *value,
                                                  const gchar **array,
                                                  gsize         length);
void           gimp_value_take_stringarray       (GValue       *value,
                                                  gchar       **array,
                                                  gsize         length);


#endif  /*  __GIMP_PARAM_SPECS_H__  */