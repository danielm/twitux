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

#include "twitux-window.h"

struct _TwituxWindowPrivate
{

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

  if (G_OBJECT_CLASS (twitux_window_parent_class)->constructed != NULL)
    G_OBJECT_CLASS (twitux_window_parent_class)->constructed (object);

  private = TWITUX_WINDOW (object)->priv;
  
  /* TODO: Setup main window here */
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
