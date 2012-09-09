/* Twitux - Tweet list viewer
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

#include "twitux-viewer.h"

struct _TwituxViewerPrivate
{
  /* Widgets here */
  GtkWidget *reserved;
};

static void twitux_viewer_class_init  (TwituxViewerClass *klass);
static void twitux_viewer_init        (TwituxViewer *viewer);

G_DEFINE_TYPE(TwituxViewer, twitux_viewer, GTK_TYPE_BOX)

static void
twitux_viewer_class_init(TwituxViewerClass *klass)
{
  g_type_class_add_private((gpointer)klass, sizeof(TwituxViewerPrivate));
}

static void
twitux_viewer_init(TwituxViewer *viewer)
{
  viewer->priv = G_TYPE_INSTANCE_GET_PRIVATE(viewer,
                                             TWITUX_TYPE_VIEWER,
                                             TwituxViewerPrivate);

  gtk_orientable_set_orientation (GTK_ORIENTABLE (viewer), GTK_ORIENTATION_VERTICAL);
  gtk_box_set_spacing (GTK_BOX (viewer), 5);
}

GtkWidget*
twitux_viewer_new (void)
{
  TwituxViewer *viewer;

  viewer = g_object_new (TWITUX_TYPE_VIEWER, NULL);

  return GTK_WIDGET (viewer);
}
