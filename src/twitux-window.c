/* Twitux - Main Window
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

#include "twitux-utils.h"
#include "twitux-window.h"
#include "twitux-viewer.h"
#include "twitux-viewer-tile.h"

struct _TwituxWindowPrivate
{
  /* Widgets here */
  GtkWidget *scrollbar;
  GtkWidget *viewer;
};

static void twitux_window_constructed (GObject *object);
static void twitux_window_class_init  (TwituxWindowClass *klass);
static void twitux_window_init        (TwituxWindow *self);

G_DEFINE_TYPE(TwituxWindow, twitux_window, GTK_TYPE_APPLICATION_WINDOW)

static void
twitux_window_class_init(TwituxWindowClass *klass)
{
  G_OBJECT_CLASS (klass)->constructed = twitux_window_constructed;

  g_type_class_add_private((gpointer)klass, sizeof(TwituxWindowPrivate));
}

static void twitux_window_init(TwituxWindow *window)
{
  window->priv = G_TYPE_INSTANCE_GET_PRIVATE(window,
                                             TWITUX_TYPE_WINDOW,
                                             TwituxWindowPrivate);
}

static void
twitux_window_constructed (GObject *object)
{
  TwituxWindowPrivate *private;
  GtkWidget *tile;
  gint i = 0;

  if (G_OBJECT_CLASS (twitux_window_parent_class)->constructed != NULL)
    G_OBJECT_CLASS (twitux_window_parent_class)->constructed (object);

  private = TWITUX_WINDOW (object)->priv;
  
  gtk_application_window_set_show_menubar (GTK_APPLICATION_WINDOW (object), TRUE);
  
  gtk_window_set_default_size (GTK_WINDOW (object), 380, 455);
  g_signal_connect (object, "delete-event",
    G_CALLBACK (gtk_widget_hide_on_delete), NULL);

  private->viewer = twitux_viewer_new ();
  
  private->scrollbar = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (private->scrollbar),
    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_add_with_viewport (
    GTK_SCROLLED_WINDOW (private->scrollbar), GTK_WIDGET (private->viewer));

  gtk_container_add (GTK_CONTAINER (object), GTK_WIDGET (private->scrollbar));

  /* Just testing data */
  for (i = 0; i <20; i++)
  {
    gchar *anonnymous = twitux_utils_lookup_file ("twitux-anonymous.png");
    
    tile = twitux_viewer_tile_new ();

    twitux_viewer_tile_set_title (TWITUX_VIEWER_TILE (tile),
      "<b>Twitux</b> <span color='#c0c0c0'>(10 minutes ago)</span>");

    twitux_viewer_tile_set_image (TWITUX_VIEWER_TILE (tile), anonnymous);

    twitux_viewer_tile_set_content (TWITUX_VIEWER_TILE (tile),
      " Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis ornare est eu lorem sollicitudin.. <a href='#'>http://tinyurl.com/twitux</a>");

    gtk_box_pack_start(GTK_BOX (private->viewer), GTK_WIDGET (tile), FALSE, FALSE, 0);

    gtk_widget_show_all (tile);
    g_free (anonnymous);
  }
}

GtkWidget*
twitux_window_new (TwituxApp *application)
{
  TwituxWindow *window;

  window = g_object_new (TWITUX_TYPE_WINDOW,
                         "application",
                         GTK_APPLICATION (application),
                         NULL);

  return GTK_WIDGET (window);
}
