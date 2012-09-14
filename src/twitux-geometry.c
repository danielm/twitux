/* Twitux - Window geometry persistency
 *
 * Copyright (C) 2012 Daniel Morales <daniminas@gmail.com>
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
#include "twitux-geometry.h"
#include "twitux-settings.h"

#define GEOMETRY_DATA_NAME "g_settings_ui"

#include <gtk/gtk.h>

static guint save_queue_id = 0;

static gboolean geometry_window_state_event_cb (GtkWindow *window,
  GdkEventWindowState *event,
  gpointer user_data);

static gboolean geometry_window_state_event_cb (GtkWindow *window,
  GdkEventWindowState *event,
  gpointer user_data);

static gboolean geometry_configure_event_cb (GtkWindow *window,
  GdkEventConfigure *event,
  gpointer user_data);

static gboolean geometry_store_cb (gpointer user_data);
static void geometry_save (GtkWindow *window);
static void geometry_load (GtkWindow *window);

void
twitux_geometry_persist (GtkWidget *window,
                         GSettings *gsettings_ui)
{
  g_object_set_data_full (G_OBJECT (window), GEOMETRY_DATA_NAME,
    g_object_ref (gsettings_ui), (GDestroyNotify) g_object_unref);

  /* Load initial metics */
  geometry_load (window);

  g_signal_connect (window, "map",
    G_CALLBACK (geometry_map_cb), NULL);
  g_signal_connect (window, "configure-event",
    G_CALLBACK (geometry_configure_event_cb), NULL);
  g_signal_connect (window, "window-state-event",
    G_CALLBACK (geometry_window_state_event_cb), NULL);
}

void
twitux_geometry_unpersist (GtkWindow *window)
{
  g_signal_handlers_disconnect_by_func (window,
    geometry_map_cb, NULL);
  g_signal_handlers_disconnect_by_func (window,
    geometry_configure_event_cb, NULL);
  g_signal_handlers_disconnect_by_func (window,
    geometry_window_state_event_cb, NULL);

  g_object_set_data (G_OBJECT (window), GEOMETRY_DATA_NAME, NULL);
}

static void
geometry_save (GtkWindow *window)
{
  if (save_queue_id != 0){
    g_source_remove (save_queue_id);
  }
  
  save_queue_id = g_timeout_add_seconds (1, geometry_store_cb, window);
}

static void
geometry_load (GtkWindow *window)
{
  GSettings *ui_settings;
  gboolean maximized;
  gint x, y, w, h;
  
  ui_settings = g_object_get_data (G_OBJECT (window), GEOMETRY_DATA_NAME);
  
  maximized = g_settings_get_boolean (ui_settings, TWITUX_UI_WINDOW_MAXIMISED);
  x = g_settings_get_int (ui_settings, TWITUX_UI_WINDOW_X_POS);
  y = g_settings_get_int (ui_settings, TWITUX_UI_WINDOW_Y_POS);
  w = g_settings_get_int (ui_settings, TWITUX_UI_WINDOW_WIDTH);
  h = g_settings_get_int (ui_settings, TWITUX_UI_WINDOW_HEIGHT);
  
  if ((x > 0) || (y > 0)){
    gtk_window_move (window, x, y);
  }
  if ((w > 0) && (h > 0)){
    gtk_window_resize (window, w, h);
  }
  
  if (maximized) {
    gtk_window_maximize (window);
  } else {
    gtk_window_unmaximize (window);
  }
}

static gboolean
geometry_store_cb (gpointer user_data)
{
  GdkWindowState window_state;
  GdkWindow *gdk_window;
  GSettings *ui_settings;
  gboolean maximized;
  gint x, y, w, h;

  if (!gtk_widget_get_visible (GTK_WIDGET (user_data))){
    return FALSE;
  }

  /* Get window metrics and status */
  gtk_window_get_position (GTK_WINDOW (user_data), &x, &y);
  gtk_window_get_size (GTK_WINDOW (user_data), &w, &h);

  gdk_window = gtk_widget_get_window (GTK_WIDGET (user_data));
  window_state = gdk_window_get_state (gdk_window);
  maximized = (window_state & GDK_WINDOW_STATE_MAXIMIZED) != 0;

  /* Save values */
  ui_settings = g_object_get_data (G_OBJECT (window), GEOMETRY_DATA_NAME);
  g_settings_set_int (ui_settings, TWITUX_UI_WINDOW_X_POS, x);
  g_settings_set_int (ui_settings, TWITUX_UI_WINDOW_Y_POS, y);
  g_settings_set_int (ui_settings, TWITUX_UI_WINDOW_WIDTH, w);
  g_settings_set_int (ui_settings, TWITUX_UI_WINDOW_HEIGHT, h);
  g_settings_set_boolean (ui_settings, TWITUX_UI_WINDOW_MAXIMISED, maximized);

  save_queue_id = 0;

  return FALSE;
}

static gboolean
geometry_configure_event_cb (GtkWindow *window,
                             GdkEventConfigure *event,
                             gpointer user_data)
{
  geometry_save (window);

  return FALSE;
}

static gboolean
geometry_window_state_event_cb (GtkWindow *window,
                                GdkEventWindowState *event,
                                gpointer user_data)
{
  if ((event->changed_mask & GDK_WINDOW_STATE_MAXIMIZED) != 0)
  {
    geometry_save (window);
  }

  return FALSE;
}

static void
geometry_map_cb (GtkWindow *window,
                 gpointer user_data)
{
  geometry_load (window);
}
