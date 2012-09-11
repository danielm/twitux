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

#include "twitux-entry-input.h"

struct _TwituxEntryInputPrivate
{
  /* Widgets here */
  GtkWidget *reserved;
};

static void twitux_entry_input_class_init  (TwituxEntryInputClass *klass);
static void twitux_entry_input_init        (TwituxEntryInput *input);

void twitux_entry_input_clear_cb (GtkWidget  *widget,
                                  gpointer    user_data);

G_DEFINE_TYPE(TwituxEntryInput, twitux_entry_input, GTK_TYPE_TEXT_VIEW)

/* signal enum */
enum
{
    STATUS_SUBMIT,
    STATUS_CLEAR,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = {0};

static void
twitux_entry_input_class_init(TwituxEntryInputClass *klass)
{
  g_type_class_add_private((gpointer)klass, sizeof(TwituxEntryInputPrivate));
  
  signals[STATUS_SUBMIT] = g_signal_new ("status_submit",
    G_TYPE_FROM_CLASS (klass),
    G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
    0,
    NULL, NULL,
    g_cclosure_marshal_VOID__VOID,
    G_TYPE_NONE, 0);
  
  signals[STATUS_CLEAR] = g_signal_new_class_handler ("status_clear",
    G_TYPE_FROM_CLASS (klass),
    G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
    G_CALLBACK (twitux_entry_input_clear_cb),
    NULL, NULL,
    g_cclosure_marshal_VOID__VOID,
    G_TYPE_NONE, 0);
}

static void
twitux_entry_input_init(TwituxEntryInput *input)
{
  GtkBindingSet *binding;

  input->priv = G_TYPE_INSTANCE_GET_PRIVATE(input,
                                            TWITUX_TYPE_ENTRY_INPUT,
                                            TwituxEntryInputPrivate);

  gtk_text_view_set_accepts_tab (GTK_TEXT_VIEW (input), TRUE);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (input), TRUE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (input), TRUE);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (input), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_left_margin (GTK_TEXT_VIEW (input), 2);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW (input), 2);
  gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (input), 2);
  gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (input), 2);
  
  binding = gtk_binding_set_by_class (GTK_TEXT_VIEW_GET_CLASS (input));

  gtk_binding_entry_add_signal (binding,
    gdk_keyval_from_name ("Return"), 0, "status_submit", 0);
  gtk_binding_entry_add_signal (binding,
    gdk_keyval_from_name ("KP_Enter"), 0, "status_submit", 0);
  gtk_binding_entry_add_signal (binding,
    gdk_keyval_from_name ("Escape"), 0, "status_clear", 0);
  
  /*
  g_signal_connect (buffer, "insert-text",
    G_CALLBACK (twitux_entry_input_insert_cb), NULL);
  */
}

void
twitux_entry_input_clear_cb (GtkWidget  *widget,
                             gpointer    user_data)
{
  GtkTextBuffer *buffer;
  
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (widget));
  
  gtk_text_buffer_set_text (buffer, "", 0);
}

GtkWidget*
twitux_entry_input_new (void)
{
  TwituxEntryInput *input;

  input = g_object_new (TWITUX_TYPE_ENTRY_INPUT, NULL);

  return GTK_WIDGET (input);
}
