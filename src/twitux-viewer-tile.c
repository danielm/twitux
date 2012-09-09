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
  GtkWidget *alignment;
};

static void twitux_viewer_tile_class_init  (TwituxViewerTileClass *klass);
static void twitux_viewer_tile_init        (TwituxViewerTile *tile);

gboolean twitux_viewer_tile_on_draw_cb (GtkWidget    *widget,
                                        cairo_t      *cr,
                                        gpointer      user_data);

G_DEFINE_TYPE(TwituxViewerTile, twitux_viewer_tile, GTK_TYPE_EVENT_BOX)

static void
twitux_viewer_tile_class_init(TwituxViewerTileClass *klass)
{
  g_type_class_add_private((gpointer)klass, sizeof(TwituxViewerTilePrivate));
}

static void
twitux_viewer_tile_init(TwituxViewerTile *tile)
{
  GtkWidget *hbox, *vbox;

  tile->priv = G_TYPE_INSTANCE_GET_PRIVATE(tile,
                                           TWITUX_TYPE_VIEWER_TILE,
                                           TwituxViewerTilePrivate);

  gtk_event_box_set_visible_window (GTK_EVENT_BOX (tile), FALSE);
  gtk_event_box_set_above_child (GTK_EVENT_BOX (tile), FALSE);
  
  tile->priv->alignment = gtk_alignment_new (0.0f, 0.0f, 1.0f, 1.0f);
  gtk_alignment_set_padding (GTK_ALIGNMENT (tile->priv->alignment), 15, 15, 15, 15);
  gtk_container_add (GTK_CONTAINER (tile), tile->priv->alignment);
  
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_container_add (GTK_CONTAINER (tile->priv->alignment), hbox);

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

  g_signal_connect (G_OBJECT (tile), "draw",
    G_CALLBACK (twitux_viewer_tile_on_draw_cb), tile);
}

gboolean
twitux_viewer_tile_on_draw_cb (GtkWidget    *widget,
                               cairo_t      *cr,
                               gpointer      user_data)
{
  TwituxViewerTile *tile = TWITUX_VIEWER_TILE (user_data);
  GtkStyleContext *context;
  GdkRGBA color;

  gdouble x             = 0,
          y             = 0,
          width         = gtk_widget_get_allocated_width(widget),
          height        = gtk_widget_get_allocated_height(widget),
          aspect        = 1.0,
          corner_radius = height / 10.0;

  gdouble radius = corner_radius / aspect;
  gdouble degrees = M_PI / 180.0;

  context = gtk_widget_get_style_context (widget);
  gtk_style_context_get_border_color (context, GTK_STATE_FLAG_NORMAL, &color);

  cairo_new_sub_path (cr);
  cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
  cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
  cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
  cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cairo_close_path (cr);

  cairo_set_source_rgba (cr, color.red, color.green, color.blue, color.alpha);
  cairo_set_line_width (cr, 10.0);
  cairo_stroke (cr);

  gtk_container_propagate_draw (GTK_CONTAINER (tile), tile->priv->alignment, cr);

  return TRUE;
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

