/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpactiongroup.c
 * Copyright (C) 2004 Michael Natterer <mitch@gimp.org>
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

#include "config.h"

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "widgets-types.h"

#include "core/gimp.h"
#include "core/gimpviewable.h"

#include "gimpactiongroup.h"
#include "gimpenumaction.h"
#include "gimppreview.h"
#include "gimpstringaction.h"

#include "gimp-intl.h"


enum
{
  PROP_0,
  PROP_GIMP
};


static void   gimp_action_group_init           (GimpActionGroup       *group);
static void   gimp_action_group_class_init     (GimpActionGroupClass  *klass);

static GObject * gimp_action_group_constructor (GType                  type,
                                                guint                  n_params,
                                                GObjectConstructParam *params);
static void   gimp_action_group_dispose        (GObject               *object);
static void   gimp_action_group_set_property   (GObject               *object,
                                                guint                  prop_id,
                                                const GValue          *value,
                                                GParamSpec            *pspec);
static void   gimp_action_group_get_property   (GObject               *object,
                                                guint                  prop_id,
                                                GValue                *value,
                                                GParamSpec            *pspec);


static GtkActionGroupClass *parent_class = NULL;


GType
gimp_action_group_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      static const GTypeInfo type_info =
      {
        sizeof (GimpActionGroupClass),
	NULL,           /* base_init */
        NULL,           /* base_finalize */
        (GClassInitFunc) gimp_action_group_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GimpActionGroup),
        0, /* n_preallocs */
        (GInstanceInitFunc) gimp_action_group_init,
      };

      type = g_type_register_static (GTK_TYPE_ACTION_GROUP,
                                     "GimpActionGroup",
				     &type_info, 0);
    }

  return type;
}

static void
gimp_action_group_class_init (GimpActionGroupClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->constructor  = gimp_action_group_constructor;
  object_class->dispose      = gimp_action_group_dispose;
  object_class->set_property = gimp_action_group_set_property;
  object_class->get_property = gimp_action_group_get_property;

  g_object_class_install_property (object_class, PROP_GIMP,
                                   g_param_spec_object ("gimp",
                                                        NULL, NULL,
                                                        GIMP_TYPE_GIMP,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY));

  klass->groups = g_hash_table_new_full (g_str_hash, g_str_equal,
                                         g_free, NULL);
}

static void
gimp_action_group_init (GimpActionGroup *group)
{
  group->gimp = NULL;
}

static GObject *
gimp_action_group_constructor (GType                  type,
                               guint                  n_params,
                               GObjectConstructParam *params)
{
  GObject     *object;
  const gchar *name;

  object = G_OBJECT_CLASS (parent_class)->constructor (type, n_params, params);

  name = gtk_action_group_get_name (GTK_ACTION_GROUP (object));

  if (name)
    {
      GimpActionGroupClass *group_class;
      GList                *list;

      group_class = GIMP_ACTION_GROUP_GET_CLASS (object);

      list = g_hash_table_lookup (group_class->groups, name);

      list = g_list_append (list, object);

      g_hash_table_replace (group_class->groups,
                            g_strdup (name), list);
    }

  return object;
}

static void
gimp_action_group_dispose (GObject *object)
{
  const gchar *name;

  name = gtk_action_group_get_name (GTK_ACTION_GROUP (object));

  if (name)
    {
      GimpActionGroupClass *group_class;
      GList                *list;

      group_class = GIMP_ACTION_GROUP_GET_CLASS (object);

      list = g_hash_table_lookup (group_class->groups, name);

      if (list)
        {
          list = g_list_remove (list, object);

          if (list)
            g_hash_table_replace (group_class->groups,
                                  g_strdup (name), list);
          else
            g_hash_table_remove (group_class->groups, name);
        }
    }

  G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void
gimp_action_group_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
  GimpActionGroup *group = GIMP_ACTION_GROUP (object);

  switch (prop_id)
    {
    case PROP_GIMP:
      group->gimp = g_value_get_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gimp_action_group_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  GimpActionGroup *group = GIMP_ACTION_GROUP (object);

  switch (prop_id)
    {
    case PROP_GIMP:
      g_value_set_object (value, group->gimp);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

/**
 * gimp_action_group_new:
 * @gimp: the @Gimp instance this action group belongs to
 * @name: the name of the action group.
 *
 * Creates a new #GimpActionGroup object. The name of the action group
 * is used when associating <link linkend="Action-Accel">keybindings</link>
 * with the actions.
 *
 * Returns: the new #GimpActionGroup
 */
GimpActionGroup *
gimp_action_group_new (Gimp                      *gimp,
                       const gchar               *name,
                       gpointer                   user_data,
                       GimpActionGroupUpdateFunc  update_func)
{
  GimpActionGroup *group;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (name != NULL, NULL);

  group = g_object_new (GIMP_TYPE_ACTION_GROUP,
                        "gimp", gimp,
                        "name", name,
                        NULL);

  group->user_data   = user_data;
  group->update_func = update_func;

  return group;
}

GList *
gimp_action_groups_from_name (const gchar *name)
{
  GimpActionGroupClass *group_class;
  GList                *list;

  g_return_val_if_fail (name != NULL, NULL);

  group_class = g_type_class_ref (GIMP_TYPE_ACTION_GROUP);

  list = g_hash_table_lookup (group_class->groups, name);

  g_type_class_unref (group_class);

  return list;
}

void
gimp_action_group_update (GimpActionGroup *group,
                          gpointer         update_data)
{
  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));

  if (group->update_func)
    group->update_func (group, update_data);
}

void
gimp_action_group_add_actions (GimpActionGroup *group,
                               GimpActionEntry *entries,
                               guint            n_entries)
{
  gint i;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));

  for (i = 0; i < n_entries; i++)
    {
      GtkAction   *action;
      const gchar *label;
      const gchar *tooltip;

      label   = gettext (entries[i].label);
      tooltip = gettext (entries[i].tooltip);

      action = gtk_action_new (entries[i].name, label, tooltip,
			       entries[i].stock_id);

      if (entries[i].callback)
        g_signal_connect (action, "activate",
                          entries[i].callback,
                          group->user_data);

      gtk_action_group_add_action_with_accel (GTK_ACTION_GROUP (group),
					      action,
					      entries[i].accelerator);
      g_object_unref (action);
    }
}

void
gimp_action_group_add_toggle_actions (GimpActionGroup       *group,
                                      GimpToggleActionEntry *entries,
                                      guint                  n_entries)
{
  gint i;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));

  for (i = 0; i < n_entries; i++)
    {
      GtkToggleAction *action;
      const gchar     *label;
      const gchar     *tooltip;

      label   = gettext (entries[i].label);
      tooltip = gettext (entries[i].tooltip);

      action = gtk_toggle_action_new (entries[i].name, label, tooltip,
				      entries[i].stock_id);

      gtk_toggle_action_set_active (action, entries[i].is_active);

      if (entries[i].callback)
        g_signal_connect (action, "toggled",
                          entries[i].callback,
                          group->user_data);

      gtk_action_group_add_action_with_accel (GTK_ACTION_GROUP (group),
					      GTK_ACTION (action),
					      entries[i].accelerator);
      g_object_unref (action);
    }
}

void
gimp_action_group_add_radio_actions (GimpActionGroup      *group,
                                     GimpRadioActionEntry *entries,
                                     guint                 n_entries,
                                     gint                  value,
                                     GCallback             callback)
{
  GtkRadioAction *first_action = NULL;
  GSList         *radio_group  = NULL;
  gint            i;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));

  for (i = 0; i < n_entries; i++)
    {
      GtkRadioAction *action;
      const gchar    *label;
      const gchar    *tooltip;

      label   = gettext (entries[i].label);
      tooltip = gettext (entries[i].tooltip);

      action = gtk_radio_action_new (entries[i].name, label, tooltip,
				     entries[i].stock_id,
				     entries[i].value);

      if (i == 0)
	first_action = action;

      gtk_radio_action_set_group (action, radio_group);
      radio_group = gtk_radio_action_get_group (action);

      if (value == entries[i].value)
	gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (action), TRUE);

      gtk_action_group_add_action_with_accel (GTK_ACTION_GROUP (group),
					      GTK_ACTION (action),
					      entries[i].accelerator);
      g_object_unref (action);
    }

  if (callback && first_action)
    g_signal_connect (first_action, "changed",
                      callback,
                      group->user_data);
}

void
gimp_action_group_add_enum_actions (GimpActionGroup     *group,
                                    GimpEnumActionEntry *entries,
                                    guint                n_entries,
                                    GCallback            callback)
{
  gint i;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));

  for (i = 0; i < n_entries; i++)
    {
      GimpEnumAction *action;
      const gchar    *label;
      const gchar    *tooltip;

      label   = gettext (entries[i].label);
      tooltip = gettext (entries[i].tooltip);

      action = gimp_enum_action_new (entries[i].name, label, tooltip,
				     entries[i].stock_id,
				     entries[i].value);

      if (callback)
        g_signal_connect (action, "selected",
                          callback,
                          group->user_data);

      gtk_action_group_add_action_with_accel (GTK_ACTION_GROUP (group),
					      GTK_ACTION (action),
					      entries[i].accelerator);
      g_object_unref (action);
    }
}

void
gimp_action_group_add_string_actions (GimpActionGroup       *group,
                                      GimpStringActionEntry *entries,
                                      guint                  n_entries,
                                      GCallback              callback)
{
  gint i;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));

  for (i = 0; i < n_entries; i++)
    {
      GimpStringAction *action;
      const gchar      *label;
      const gchar      *tooltip;

      label   = gettext (entries[i].label);
      tooltip = gettext (entries[i].tooltip);

      action = gimp_string_action_new (entries[i].name, label, tooltip,
                                       entries[i].stock_id,
                                       entries[i].value);

      if (callback)
        g_signal_connect (action, "selected",
                          callback,
                          group->user_data);

      gtk_action_group_add_action_with_accel (GTK_ACTION_GROUP (group),
					      GTK_ACTION (action),
					      entries[i].accelerator);
      g_object_unref (action);
    }
}

void
gimp_action_group_set_action_visible (GimpActionGroup *group,
                                      const gchar     *action_name,
                                      gboolean         visible)
{
  GtkAction *action;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (action_name != NULL);

  action = gtk_action_group_get_action (GTK_ACTION_GROUP (group), action_name);

  if (! action)
    {
      g_warning ("%s: Unable to set visibility of action "
                 "which doesn't exist: %s",
                 G_STRLOC, action_name);
      return;
    }

  g_object_set (action, "visible", visible ? TRUE : FALSE, NULL);
}

void
gimp_action_group_set_action_sensitive (GimpActionGroup *group,
                                        const gchar     *action_name,
                                        gboolean         sensitive)
{
  GtkAction *action;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (action_name != NULL);

  action = gtk_action_group_get_action (GTK_ACTION_GROUP (group), action_name);

  if (! action)
    {
      g_warning ("%s: Unable to set sensitivity of action "
                 "which doesn't exist: %s",
                 G_STRLOC, action_name);
      return;
    }

  g_object_set (action, "sensitive", sensitive ? TRUE : FALSE, NULL);
}

void
gimp_action_group_set_action_active (GimpActionGroup *group,
                                     const gchar     *action_name,
                                     gboolean         active)
{
  GtkAction *action;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (action_name != NULL);

  action = gtk_action_group_get_action (GTK_ACTION_GROUP (group), action_name);

  if (! action)
    {
      g_warning ("%s: Unable to set \"active\" of action "
                 "which doesn't exist: %s",
                 G_STRLOC, action_name);
      return;
    }

  if (! GTK_IS_TOGGLE_ACTION (action))
    {
      g_warning ("%s: Unable to set \"active\" of action "
                 "which is not a GtkToggleAction: %s",
                 G_STRLOC, action_name);
      return;
    }

  gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (action),
                                active ? TRUE : FALSE);
}

void
gimp_action_group_set_action_label (GimpActionGroup *group,
                                    const gchar     *action_name,
                                    const gchar     *label)
{
  GtkAction *action;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (action_name != NULL);

  action = gtk_action_group_get_action (GTK_ACTION_GROUP (group), action_name);

  if (! action)
    {
      g_warning ("%s: Unable to set label of action "
                 "which doesn't exist: %s",
                 G_STRLOC, action_name);
      return;
    }

  g_object_set (action, "label", label, NULL);
}

void
gimp_action_group_set_action_color (GimpActionGroup *group,
                                    const gchar     *action_name,
                                    const GimpRGB   *color,
                                    gboolean         set_label)
{
  GtkAction *action;
  GSList    *list;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (action_name != NULL);
  g_return_if_fail (color != NULL);

  action = gtk_action_group_get_action (GTK_ACTION_GROUP (group), action_name);

  if (! action)
    {
      g_warning ("%s: Unable to set color of action "
                 "which doesn't exist: %s",
                 G_STRLOC, action_name);
      return;
    }

  if (set_label)
    {
      gchar *label = g_strdup_printf (_("RGBA (%0.3f, %0.3f, %0.3f, %0.3f)"),
                                      color->r, color->g, color->b, color->a);

      g_object_set (action, "label", label, NULL);
      g_free (label);
    }

  for (list = gtk_action_get_proxies (action); list; list = g_slist_next (list))
    {
      GtkWidget *widget = list->data;
      GtkWidget *area   = NULL;

      if (! GTK_IS_MENU_ITEM (widget))
        continue;

      if (GTK_IS_HBOX (GTK_BIN (widget)->child))
        {
          area = g_object_get_data (G_OBJECT (GTK_BIN (widget)->child),
                                    "gimp-color-area");
        }
      else if (GTK_IS_LABEL (GTK_BIN (widget)->child))
        {
          GdkScreen *screen;
          GtkWidget *label;
          GtkWidget *hbox;
          gint       width, height;

          label = GTK_BIN (widget)->child;

          g_object_ref (label);

          gtk_container_remove (GTK_CONTAINER (widget), label);

          hbox = gtk_hbox_new (FALSE, 4);
          gtk_container_add (GTK_CONTAINER (widget), hbox);
          gtk_widget_show (hbox);

          screen = gtk_widget_get_screen (widget);
          gtk_icon_size_lookup_for_settings (gtk_settings_get_for_screen (screen),
                                             GTK_ICON_SIZE_MENU,
                                             &width, &height);

          area = gimp_color_area_new (color, GIMP_COLOR_AREA_SMALL_CHECKS, 0);
          gimp_color_area_set_draw_border (GIMP_COLOR_AREA (area), TRUE);
          gtk_widget_set_size_request (area, width, height);
          gtk_box_pack_start (GTK_BOX (hbox), area, FALSE, FALSE, 0);
          gtk_widget_show (area);

          gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
          gtk_widget_show (label);

          g_object_unref (label);

          g_object_set_data (G_OBJECT (hbox), "gimp-color-area", area);
        }

      if (area)
        gimp_color_area_set_color (GIMP_COLOR_AREA (area), color);
    }
}

void
gimp_action_group_set_action_viewable (GimpActionGroup *group,
                                       const gchar     *action_name,
                                       GimpViewable    *viewable)
{
  GtkAction *action;
  GSList    *list;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (action_name != NULL);
  g_return_if_fail (GIMP_IS_VIEWABLE (viewable));

  action = gtk_action_group_get_action (GTK_ACTION_GROUP (group), action_name);

  if (! action)
    {
      g_warning ("%s: Unable to set viewable of action "
                 "which doesn't exist: %s",
                 G_STRLOC, action_name);
      return;
    }

  for (list = gtk_action_get_proxies (action); list; list = g_slist_next (list))
    {
      GtkWidget *widget  = list->data;
      GtkWidget *preview = NULL;

      if (! GTK_IS_MENU_ITEM (widget))
        continue;

      if (GTK_IS_HBOX (GTK_BIN (widget)->child))
        {
          preview = g_object_get_data (G_OBJECT (GTK_BIN (widget)->child),
                                       "gimp-preview");
        }
      else if (GTK_IS_LABEL (GTK_BIN (widget)->child))
        {
          GdkScreen *screen;
          GtkWidget *label;
          GtkWidget *hbox;
          gint       width, height;

          label = GTK_BIN (widget)->child;

          g_object_ref (label);

          gtk_container_remove (GTK_CONTAINER (widget), label);

          hbox = gtk_hbox_new (FALSE, 4);
          gtk_container_add (GTK_CONTAINER (widget), hbox);
          gtk_widget_show (hbox);

          screen = gtk_widget_get_screen (widget);
          gtk_icon_size_lookup_for_settings (gtk_settings_get_for_screen (screen),
                                             GTK_ICON_SIZE_MENU,
                                             &width, &height);

          preview = gimp_preview_new_full (viewable, width - 2, height - 2, 1,
                                           FALSE, FALSE, FALSE);
          gtk_box_pack_start (GTK_BOX (hbox), preview, FALSE, FALSE, 0);
          gtk_widget_show (preview);

          gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
          gtk_widget_show (label);

          g_object_unref (label);

          g_object_set_data (G_OBJECT (hbox), "gimp-preview", preview);
        }

      if (preview)
        gimp_preview_set_viewable (GIMP_PREVIEW (preview), viewable);
    }
}

void
gimp_action_group_set_action_important (GimpActionGroup *group,
                                        const gchar     *action_name,
                                        gboolean         is_important)
{
  GtkAction *action;

  g_return_if_fail (GIMP_IS_ACTION_GROUP (group));
  g_return_if_fail (action_name != NULL);

  action = gtk_action_group_get_action (GTK_ACTION_GROUP (group), action_name);

  if (! action)
    {
      g_warning ("%s: Unable to set \"is-important\" of action "
                 "which doesn't exist: %s",
                 G_STRLOC, action_name);
      return;
    }

  g_object_set (action, "is-important", is_important ? TRUE : FALSE, NULL);
}
