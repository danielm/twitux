/* Twitux - Tweet list viewer tile
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

#include <math.h>
#include <gdk/gdk.h>

#include "twitux-viewer-tile.h"

struct _TwituxViewerTilePrivate
{
  /* Widgets here */
  GtkWidget *picture;
  GtkWidget *title;
  GtkWidget *content;
};

static void twitux_viewer_tile_class_init  (TwituxViewerTileClass *klass);
static void twitux_viewer_tile_init        (TwituxViewerTile *tile);

G_DEFINE_TYPE(TwituxViewerTile, twitux_viewer_tile, GTK_TYPE_EVENT_BOX)

static void
twitux_viewer_tile_class_init(TwituxViewerTileClass *klass)
{
  g_type_class_add_private((gpointer)klass, sizeof(TwituxViewerTilePrivate));
}

static void
twitux_viewer_tile_init(TwituxViewerTile *tile)
{
  GtkWidget *alignment;
  GtkWidget *hbox, *vbox;

  tile->priv = G_TYPE_INSTANCE_GET_PRIVATE(tile,
                                           TWITUX_TYPE_VIEWER_TILE,
                                           TwituxViewerTilePrivate);

  gtk_event_box_set_visible_window (GTK_EVENT_BOX (tile), TRUE);
  
  alignment = gtk_alignment_new (0.0f, 0.0f, 1.0f, 1.0f);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 15, 15, 15, 15);
  gtk_container_add (GTK_CONTAINER (tile), alignment);
  
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_container_add (GTK_CONTAINER (alignment), hbox);

  tile->priv->picture = gtk_image_new ();
  gtk_misc_set_alignment (GTK_MISC (tile->priv->picture), 0.0f, 0.0f);
  gtk_misc_set_padding (GTK_MISC (tile->priv->picture), 0, 0);
  gtk_box_pack_start (GTK_BOX (hbox), tile->priv->picture, FALSE, FALSE, 0);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox, FALSE, FALSE, 0);

  tile->priv->title = gtk_label_new ("");
  gtk_misc_set_alignment (GTK_MISC (tile->priv->title), 0.0f, 0.0f);
  gtk_label_set_use_markup (GTK_LABEL (tile->priv->title), TRUE);
  gtk_misc_set_padding (GTK_MISC (tile->priv->title), 0, 0);
  gtk_box_pack_start (GTK_BOX (vbox), tile->priv->title, FALSE, FALSE, 0);
	
  tile->priv->content = gtk_label_new ("");
  gtk_label_set_use_markup (GTK_LABEL (tile->priv->content), TRUE);
  gtk_label_set_line_wrap (GTK_LABEL (tile->priv->content), TRUE);
  gtk_label_set_track_visited_links (GTK_LABEL (tile->priv->content), FALSE);
  gtk_misc_set_alignment (GTK_MISC (tile->priv->content), 0.0f, 0.0f);
  gtk_box_pack_start (GTK_BOX (vbox), tile->priv->content, FALSE, FALSE, 0);
}

GtkWidget*
twitux_viewer_tile_new (void) 
{
  TwituxViewerTile *tile;

  tile = g_object_new (TWITUX_TYPE_VIEWER_TILE, NULL);

  return GTK_WIDGET (tile);
}

void
twitux_viewer_tile_set_title (TwituxViewerTile *tile,
                              const gchar      *markup)
{
  gtk_label_set_markup (GTK_LABEL (tile->priv->title), markup);
}

void
twitux_viewer_tile_set_content (TwituxViewerTile *tile,
                                const gchar      *markup)
{
  gtk_label_set_markup (GTK_LABEL (tile->priv->content), markup);
}

void
twitux_viewer_tile_set_image (TwituxViewerTile *tile,
                              const gchar      *filename)
{
  gtk_image_set_from_file (GTK_IMAGE (tile->priv->picture), filename);
}
