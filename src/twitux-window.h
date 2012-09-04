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

#ifndef __TWITUX_WINDOW_H__
#define __TWITUX_WINDOW_H__

#include "twitux-app.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TWITUX_TYPE_WINDOW               (twitux_window_get_type ())
#define TWITUX_WINDOW(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj), TWITUX_TYPE_WINDOW, TwituxWindow))
#define TWITUX_WINDOW_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST((klass), TWITUX_TYPE_WINDOW, TwituxWindowClass))
#define TWITUX_IS_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj), TWITUX_TYPE_WINDOW))
#define TWITUX_IS_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE((klass), TWITUX_TYPE_WINDOW))
#define TWITUX_WINDOW_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS((obj), TWITUX_TYPE_WINDOW, TwituxWindowClass))

typedef struct _TwituxWindow             TwituxWindow;
typedef struct _TwituxWindowClass        TwituxWindowClass;
typedef struct _TwituxWindowPrivate      TwituxWindowPrivate;

struct _TwituxWindow
{
  GtkApplicationWindow parent;

  TwituxWindowPrivate *priv;
};

struct _TwituxWindowClass
{
  GtkApplicationWindowClass parent_class;
};


GType        twitux_window_get_type      (void);
GtkWidget*   twitux_window_new           (TwituxApp *app);

G_END_DECLS

#endif /* __TWITUX_WINDOW_H__ */
