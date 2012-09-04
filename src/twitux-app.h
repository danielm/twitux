/* Twitux - Application Facade
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
 
#ifndef _TWITUX_APP_H_
#define _TWITUX_APP_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TWITUX_TYPE_APP                (twitux_application_get_type ())
#define TWITUX_APP(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), TWITUX_TYPE_APP, TwituxApp))
#define TWITUX_APP_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), TWITUX_TYPE_APP, TwituxAppClass))
#define TWITUX_IS_APP(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TWITUX_TYPE_APP))
#define TWITUX_IS_APP_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), TWITUX_TYPE_APP))
#define TWITUX_APP_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), TWITUX_TYPE_APP, TwituxAppClass))

typedef struct _TwituxAppClass         TwituxAppClass;
typedef struct _TwituxApp              TwituxApp;
typedef struct _TwituxAppPrivate       TwituxAppPrivate;

struct _TwituxAppClass
{
  GtkApplicationClass parent_class;
};

struct _TwituxApp
{
  GtkApplication parent_instance;

  TwituxAppPrivate *priv;
};

GType      twitux_application_get_type (void) G_GNUC_CONST;
TwituxApp* twitux_application_new      (void);

G_END_DECLS

#endif /* _TWITUX_APPLICATION_H_ */
