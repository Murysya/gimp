/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpdialog.c
 * Copyright (C) 2000-2003 Michael Natterer <mitch@gimp.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <gtk/gtk.h>

#include "gimpwidgetstypes.h"

#include "gimpdialog.h"
#include "gimphelpui.h"


enum
{
  PROP_0,
  PROP_HELP_FUNC,
  PROP_HELP_ID
};


static GObject  * gimp_dialog_constructor  (GType            type,
                                            guint            n_params,
                                            GObjectConstructParam *params);
static void       gimp_dialog_dispose      (GObject         *object);
static void       gimp_dialog_set_property (GObject         *object,
                                            guint            property_id,
                                            const GValue    *value,
                                            GParamSpec      *pspec);
static void       gimp_dialog_get_property (GObject         *object,
                                            guint            property_id,
                                            GValue          *value,
                                            GParamSpec      *pspec);

static void       gimp_dialog_hide         (GtkWidget       *widget);
static gboolean   gimp_dialog_delete_event (GtkWidget       *widget,
                                            GdkEventAny     *event);
static void       gimp_dialog_close        (GtkDialog       *dialog);

static void       gimp_dialog_help         (GObject         *dialog);
static void       gimp_dialog_response     (GtkDialog       *dialog,
                                            gint             response_id);


G_DEFINE_TYPE (GimpDialog, gimp_dialog, GTK_TYPE_DIALOG)

#define parent_class gimp_dialog_parent_class

static gboolean show_help_button = TRUE;


static void
gimp_dialog_class_init (GimpDialogClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GtkDialogClass *dialog_class = GTK_DIALOG_CLASS (klass);

  object_class->constructor  = gimp_dialog_constructor;
  object_class->dispose      = gimp_dialog_dispose;
  object_class->set_property = gimp_dialog_set_property;
  object_class->get_property = gimp_dialog_get_property;

  widget_class->hide         = gimp_dialog_hide;
  widget_class->delete_event = gimp_dialog_delete_event;

  dialog_class->close        = gimp_dialog_close;

  /**
   * GimpDialog::help-func:
   *
   * Since: GIMP 2.2
   **/
  g_object_class_install_property (object_class, PROP_HELP_FUNC,
                                   g_param_spec_pointer ("help-func", NULL, NULL,
                                                         GIMP_PARAM_READWRITE |
                                                         G_PARAM_CONSTRUCT_ONLY));

  /**
   * GimpDialog::help-id:
   *
   * Since: GIMP 2.2
   **/
  g_object_class_install_property (object_class, PROP_HELP_ID,
                                   g_param_spec_string ("help-id", NULL, NULL,
                                                        NULL,
                                                        GIMP_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY));
}

static void
gimp_dialog_init (GimpDialog *dialog)
{
  gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);

  g_signal_connect (dialog, "response",
                    G_CALLBACK (gimp_dialog_response),
                    NULL);
}

static GObject *
gimp_dialog_constructor (GType                  type,
                         guint                  n_params,
                         GObjectConstructParam *params)
{
  GObject      *object;
  GimpHelpFunc  help_func;
  const gchar  *help_id;

  object = G_OBJECT_CLASS (parent_class)->constructor (type, n_params, params);

  help_func = g_object_get_data (object, "gimp-dialog-help-func");
  help_id   = g_object_get_data (object, "gimp-dialog-help-id");

  if (help_func)
    gimp_help_connect (GTK_WIDGET (object), help_func, help_id, object);

  if (show_help_button && help_func && help_id)
    {
      GtkDialog *dialog = GTK_DIALOG (object);
      GtkWidget *button = gtk_button_new_from_stock (GTK_STOCK_HELP);

      gtk_box_pack_end (GTK_BOX (dialog->action_area), button, FALSE, TRUE, 0);
      gtk_button_box_set_child_secondary (GTK_BUTTON_BOX (dialog->action_area),
                                          button, TRUE);
      gtk_widget_show (button);

      g_signal_connect_object (button, "clicked",
                               G_CALLBACK (gimp_dialog_help),
                               dialog, G_CONNECT_SWAPPED);

      g_object_set_data (object, "gimp-dialog-help-button", button);
    }

  return object;
}

static void
gimp_dialog_dispose (GObject *object)
{
  GdkDisplay *display = NULL;

  if (g_main_depth () == 0)
    {
      display = gtk_widget_get_display (GTK_WIDGET (object));
      g_object_ref (display);
    }

  G_OBJECT_CLASS (parent_class)->dispose (object);

  if (display)
    {
      gdk_display_flush (display);
      g_object_unref (display);
    }
}

static void
gimp_dialog_set_property (GObject      *object,
                          guint         property_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
  switch (property_id)
    {
    case PROP_HELP_FUNC:
      g_object_set_data (object, "gimp-dialog-help-func",
                         g_value_get_pointer (value));
      break;
    case PROP_HELP_ID:
      g_object_set_data_full (object, "gimp-dialog-help-id",
                              g_value_dup_string (value),
                              (GDestroyNotify) g_free);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_dialog_get_property (GObject    *object,
                          guint       property_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
  switch (property_id)
    {
    case PROP_HELP_FUNC:
      g_value_set_pointer (value, g_object_get_data (object,
                                                     "gimp-dialog-help-func"));
      break;
    case PROP_HELP_ID:
      g_value_set_string (value, g_object_get_data (object,
                                                    "gimp-dialog-help-id"));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_dialog_hide (GtkWidget *widget)
{
  /*  set focus to NULL so focus_out callbacks are invoked synchronously  */
  gtk_window_set_focus (GTK_WINDOW (widget), NULL);

  GTK_WIDGET_CLASS (parent_class)->hide (widget);
}

static gboolean
gimp_dialog_delete_event (GtkWidget   *widget,
                          GdkEventAny *event)
{
  return TRUE;
}

static void
gimp_dialog_close (GtkDialog *dialog)
{
  /* Synthesize delete_event to close dialog. */

  GtkWidget *widget = GTK_WIDGET (dialog);

  if (widget->window)
    {
      GdkEvent *event = gdk_event_new (GDK_DELETE);

      event->any.window     = g_object_ref (widget->window);
      event->any.send_event = TRUE;

      gtk_main_do_event (event);
      gdk_event_free (event);
    }
}

static void
gimp_dialog_help (GObject *dialog)
{
  GimpHelpFunc  help_func = g_object_get_data (dialog, "gimp-dialog-help-func");

  if (help_func)
    help_func (g_object_get_data (dialog, "gimp-dialog-help-id"), dialog);
}

static void
gimp_dialog_response (GtkDialog *dialog,
                      gint       response_id)
{
  GList *children;
  GList *list;

  children = gtk_container_get_children (GTK_CONTAINER (dialog->action_area));

  for (list = children; list; list = g_list_next (list))
    {
      GtkWidget *widget = list->data;

      if (gtk_dialog_get_response_for_widget (dialog, widget) == response_id)
        {
          if (! GTK_IS_BUTTON (widget) ||
              gtk_button_get_focus_on_click (GTK_BUTTON (widget)))
            {
              gtk_widget_grab_focus (widget);
            }

          break;
        }
    }

  g_list_free (children);
}


/**
 * gimp_dialog_new:
 * @title:        The dialog's title which will be set with
 *                gtk_window_set_title().
 * @role:         The dialog's @role which will be set with
 *                gtk_window_set_role().
 * @parent:       The @parent widget of this dialog.
 * @flags:        The @flags (see the #GtkDialog documentation).
 * @help_func:    The function which will be called if the user presses "F1".
 * @help_id:      The help_id which will be passed to @help_func.
 * @...:          A %NULL-terminated @va_list destribing the
 *                action_area buttons.
 *
 * Creates a new @GimpDialog widget.
 *
 * This function simply packs the action_area arguments passed in "..."
 * into a @va_list variable and passes everything to gimp_dialog_new_valist().
 *
 * For a description of the format of the @va_list describing the
 * action_area buttons see gtk_dialog_new_with_buttons().
 *
 * Returns: A #GimpDialog.
 **/
GtkWidget *
gimp_dialog_new (const gchar    *title,
                 const gchar    *role,
                 GtkWidget      *parent,
                 GtkDialogFlags  flags,
                 GimpHelpFunc    help_func,
                 const gchar    *help_id,
                 ...)
{
  GtkWidget *dialog;
  va_list    args;

  g_return_val_if_fail (parent == NULL || GTK_IS_WIDGET (parent), NULL);
  g_return_val_if_fail (title != NULL, NULL);
  g_return_val_if_fail (role != NULL, NULL);

  va_start (args, help_id);

  dialog = gimp_dialog_new_valist (title, role,
                                   parent, flags,
                                   help_func, help_id,
                                   args);

  va_end (args);

  return dialog;
}

/**
 * gimp_dialog_new_valist:
 * @title:        The dialog's title which will be set with
 *                gtk_window_set_title().
 * @role:         The dialog's @role which will be set with
 *                gtk_window_set_role().
 * @parent:       The @parent widget of this dialog or %NULL.
 * @flags:        The @flags (see the #GtkDialog documentation).
 * @help_func:    The function which will be called if the user presses "F1".
 * @help_id:      The help_id which will be passed to @help_func.
 * @args:         A @va_list destribing the action_area buttons.
 *
 * Creates a new @GimpDialog widget. If a GtkWindow is specified as
 * @parent then the dialog will be made transient for this window.
 *
 * For a description of the format of the @va_list describing the
 * action_area buttons see gtk_dialog_new_with_buttons().
 *
 * Returns: A #GimpDialog.
 **/
GtkWidget *
gimp_dialog_new_valist (const gchar    *title,
                        const gchar    *role,
                        GtkWidget      *parent,
                        GtkDialogFlags  flags,
                        GimpHelpFunc    help_func,
                        const gchar    *help_id,
                        va_list         args)
{
  GtkWidget *dialog;

  g_return_val_if_fail (title != NULL, NULL);
  g_return_val_if_fail (role != NULL, NULL);
  g_return_val_if_fail (parent == NULL || GTK_IS_WIDGET (parent), NULL);

  dialog = g_object_new (GIMP_TYPE_DIALOG,
                         "title",     title,
                         "role",      role,
                         "modal",     (flags & GTK_DIALOG_MODAL),
                         "help-func", help_func,
                         "help-id",   help_id,
                         NULL);

  if (parent)
    {
      if (GTK_IS_WINDOW (parent))
        {
          gtk_window_set_transient_for (GTK_WINDOW (dialog),
                                        GTK_WINDOW (parent));
        }
      else
        {
          gtk_window_set_screen (GTK_WINDOW (dialog),
                                 gtk_widget_get_screen (parent));
        }

      if (flags & GTK_DIALOG_DESTROY_WITH_PARENT)
        g_signal_connect_object (parent, "destroy",
                                 G_CALLBACK (gimp_dialog_close),
                                 dialog, G_CONNECT_SWAPPED);
    }

  gimp_dialog_add_buttons_valist (GIMP_DIALOG (dialog), args);

  return dialog;
}

/**
 * gimp_dialog_add_button:
 * @dialog: The @dialog to add a button to.
 * @button_text: text of button, or stock ID.
 * @response_id: response ID for the button.
 *
 * This function is essentially the same as gtk_dialog_add_button()
 * except it ensures there is only one help button and automatically
 * sets the RESPONSE_OK widget as the default response.
 *
 * Return value: the button widget that was added.
 **/
GtkWidget *
gimp_dialog_add_button (GimpDialog  *dialog,
                        const gchar *button_text,
                        gint         response_id)
{
  GtkWidget *button;

  /*  hide the automatically added help button if another one is added  */
  if (response_id == GTK_RESPONSE_HELP)
    {
      GtkWidget *help_button = g_object_get_data (G_OBJECT (dialog),
                                                  "gimp-dialog-help-button");
      if (help_button)
        gtk_widget_hide (help_button);
    }

  button = gtk_dialog_add_button (GTK_DIALOG (dialog), button_text,
                                  response_id);

  if (response_id == GTK_RESPONSE_OK)
    {
      gtk_dialog_set_default_response (GTK_DIALOG (dialog),
                                       GTK_RESPONSE_OK);
    }

  return button;
}

/**
 * gimp_dialog_add_buttons:
 * @dialog: The @dialog to add buttons to.
 * @Varargs: button_text-response_id pairs.
 *
 * This function is essentially the same as gtk_dialog_add_buttons()
 * except it calls gimp_dialog_add_button() instead of gtk_dialog_add_button()
 **/
void
gimp_dialog_add_buttons (GimpDialog *dialog,
                         ...)
{
  va_list args;

  va_start (args, dialog);

  gimp_dialog_add_buttons_valist (dialog, args);

  va_end (args);
}

/**
 * gimp_dialog_add_buttons_valist:
 * @dialog: The @dialog to add buttons to.
 * @args:   The buttons as va_list.
 *
 * This function is essentially the same as gimp_dialog_add_buttons()
 * except it takes a va_list instead of '...'
 **/
void
gimp_dialog_add_buttons_valist (GimpDialog *dialog,
                                va_list     args)
{
  const gchar *button_text;
  gint         response_id;

  g_return_if_fail (GIMP_IS_DIALOG (dialog));

  while ((button_text = va_arg (args, const gchar *)))
    {
      response_id = va_arg (args, gint);

      gimp_dialog_add_button (dialog, button_text, response_id);
    }
}


typedef struct
{
  GtkDialog *dialog;
  gint       response_id;
  GMainLoop *loop;
  gboolean   destroyed;
} RunInfo;

static void
run_shutdown_loop (RunInfo *ri)
{
  if (g_main_loop_is_running (ri->loop))
    g_main_loop_quit (ri->loop);
}

static void
run_unmap_handler (GtkDialog *dialog,
                   RunInfo   *ri)
{
  run_shutdown_loop (ri);
}

static void
run_response_handler (GtkDialog *dialog,
                      gint       response_id,
                      RunInfo   *ri)
{
  ri->response_id = response_id;

  run_shutdown_loop (ri);
}

static gint
run_delete_handler (GtkDialog   *dialog,
                    GdkEventAny *event,
                    RunInfo     *ri)
{
  run_shutdown_loop (ri);

  return TRUE; /* Do not destroy */
}

static void
run_destroy_handler (GtkDialog *dialog,
                     RunInfo   *ri)
{
  /* shutdown_loop will be called by run_unmap_handler */

  ri->destroyed = TRUE;
}

/**
 * gimp_dialog_run:
 * @dialog: a #GimpDialog
 *
 * This function does exactly the same as gtk_dialog_run() except it
 * does not make the dialog modal while the #GMainLoop is running.
 *
 * Return value: response ID
 **/
gint
gimp_dialog_run (GimpDialog *dialog)
{
  RunInfo ri = { NULL, GTK_RESPONSE_NONE, NULL };
  gulong  response_handler;
  gulong  unmap_handler;
  gulong  destroy_handler;
  gulong  delete_handler;

  g_return_val_if_fail (GIMP_IS_DIALOG (dialog), -1);

  g_object_ref (dialog);

  gtk_window_present (GTK_WINDOW (dialog));

  response_handler = g_signal_connect (dialog, "response",
                                       G_CALLBACK (run_response_handler),
                                       &ri);
  unmap_handler    = g_signal_connect (dialog, "unmap",
                                       G_CALLBACK (run_unmap_handler),
                                       &ri);
  delete_handler   = g_signal_connect (dialog, "delete-event",
                                       G_CALLBACK (run_delete_handler),
                                       &ri);
  destroy_handler  = g_signal_connect (dialog, "destroy",
                                       G_CALLBACK (run_destroy_handler),
                                       &ri);

  ri.loop = g_main_loop_new (NULL, FALSE);

  GDK_THREADS_LEAVE ();
  g_main_loop_run (ri.loop);
  GDK_THREADS_ENTER ();

  g_main_loop_unref (ri.loop);

  ri.loop      = NULL;
  ri.destroyed = FALSE;

  if (!ri.destroyed)
    {
      g_signal_handler_disconnect (dialog, response_handler);
      g_signal_handler_disconnect (dialog, unmap_handler);
      g_signal_handler_disconnect (dialog, delete_handler);
      g_signal_handler_disconnect (dialog, destroy_handler);
    }

  g_object_unref (dialog);

  return ri.response_id;
}

/**
 * gimp_dialogs_show_help_button:
 * @show: whether a help button should be added when creating a GimpDialog
 *
 * This function is for internal use only.
 *
 * Since: GIMP 2.2
 **/
void
gimp_dialogs_show_help_button (gboolean  show)
{
  show_help_button = show ? TRUE : FALSE;
}
