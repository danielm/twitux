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

#ifndef __TWITUX_VIEWER_H__
#define __TWITUX_VIEWER_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TWITUX_TYPE_VIEWER               (twitux_viewer_get_type ())
#define TWITUX_VIEWER(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj), TWITUX_TYPE_VIEWER, TwituxViewer))
#define TWITUX_VIEWER_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST((klass), TWITUX_TYPE_VIEWER, TwituxViewerClass))
#define TWITUX_IS_VIEWER(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj), TWITUX_TYPE_VIEWER))
#define TWITUX_IS_VIEWER_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE((klass), TWITUX_TYPE_VIEWER))
#define TWITUX_VIEWER_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS((obj), TWITUX_TYPE_VIEWER, TwituxViewerClass))

typedef struct _TwituxViewer             TwituxViewer;
typedef struct _TwituxViewerClass        TwituxViewerClass;
typedef struct _TwituxViewerPrivate      TwituxViewerPrivate;

struct _TwituxViewer
{
  GtkBox parent;

  TwituxViewerPrivate *priv;
};

struct _TwituxViewerClass
{
  GtkBoxClass parent_class;
};


GType        twitux_viewer_get_type      (void);
GtkWidget*   twitux_viewer_new           (void);

G_END_DECLS

#endif /* __TWITUX_VIEWER_H__ */
