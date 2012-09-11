/* Twitux - Update status widget (input)
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

#ifndef __TWITUX_ENTRY_INPUT_H__
#define __TWITUX_ENTRY_INPUT_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TWITUX_TYPE_ENTRY_INPUT               (twitux_entry_input_get_type ())
#define TWITUX_ENTRY_INPUT(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj), TWITUX_TYPE_ENTRY_INPUT, TwituxEntryInput))
#define TWITUX_ENTRY_INPUT_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST((klass), TWITUX_TYPE_ENTRY_INPUT, TwituxEntryInputClass))
#define TWITUX_IS_ENTRY_INPUT(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj), TWITUX_TYPE_ENTRY_INPUT))
#define TWITUX_IS_ENTRY_INPUT_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE((klass), TWITUX_TYPE_ENTRY_INPUT))
#define TWITUX_ENTRY_INPUT_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS((obj), TWITUX_TYPE_ENTRY_INPUT, TwituxEntryInputClass))

typedef struct _TwituxEntryInput              TwituxEntryInput;
typedef struct _TwituxEntryInputClass         TwituxEntryInputClass;
typedef struct _TwituxEntryInputPrivate       TwituxEntryInputPrivate;

struct _TwituxEntryInput
{
  GtkBox parent;

  TwituxEntryInputPrivate *priv;
};

struct _TwituxEntryInputClass
{
  GtkTextViewClass parent_class;
};


GType        twitux_entry_input_get_type      (void);
GtkWidget*   twitux_entry_input_new           (void);

G_END_DECLS

#endif /* __TWITUX_ENTRY_INPUT_H__ */
