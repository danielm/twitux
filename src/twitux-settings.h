/* Twitux - Application Settings paths
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
 
#ifndef __TWITUX_SETTINGS_H__
#define __TWITUX_SETTINGS_H__

#include <gio/gio.h>

G_BEGIN_DECLS

#define TWITUX_SCHEMA "org.gnome.Twitux"


#define TWITUX_SCHEMA_UI TWITUX_SCHEMA ".ui"
#define TWITUX_UI_WINDOW_MAXIMISED     "window-maximised"
#define TWITUX_UI_WINDOW_X_POS         "window-x-pos"
#define TWITUX_UI_WINDOW_Y_POS         "window-y-pos"
#define TWITUX_UI_WINDOW_WIDTH         "window-width"
#define TWITUX_UI_WINDOW_HEIGHT        "window-height"

G_END_DECLS

#endif /* __TWITUX_SETTINGS_H__ */
