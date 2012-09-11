/* Twitux - Update status widget
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

#include "twitux-entry.h"
#include "twitux-entry-input.h"

#include <glib/gi18n.h>

#define MAX_CHARACTER_COUNT 140

struct _TwituxEntryPrivate
{
  /* Widgets here */
  GtkWidget *input;
  
  GtkWidget *counter;
};

static void twitux_entry_class_init  (TwituxEntryClass *klass);
static void twitux_entry_init        (TwituxEntry *entry);

void twitux_entry_submit_cb (GtkWidget *widget,
                             gpointer   user_data);

void twitux_entry_changed_cb (GtkTextBuffer *textbuffer,
                              gpointer       user_data);

G_DEFINE_TYPE(TwituxEntry, twitux_entry, GTK_TYPE_BOX)

static void
twitux_entry_class_init(TwituxEntryClass *klass)
{
  g_type_class_add_private((gpointer)klass, sizeof(TwituxEntryPrivate));
}

static void
twitux_entry_init(TwituxEntry *entry)
{
  GtkTextBuffer *buffer;
  GtkWidget *scroll, *hbox;
  const gchar *standard_msg;
  gchar *character_count;

  entry->priv = G_TYPE_INSTANCE_GET_PRIVATE(entry,
                                            TWITUX_TYPE_ENTRY,
                                            TwituxEntryPrivate);

  gtk_orientable_set_orientation (GTK_ORIENTABLE (entry), GTK_ORIENTATION_VERTICAL);
  gtk_box_set_spacing (GTK_BOX (entry), 0);
  
  /* Scroll */
  scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scroll),
    GTK_SHADOW_IN);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start (GTK_BOX (entry), scroll, TRUE, TRUE, 0);

  /* Viewer */
  entry->priv->input = twitux_entry_input_new ();
  gtk_container_add (GTK_CONTAINER (scroll), entry->priv->input);

  g_signal_connect (G_OBJECT (entry->priv->input), "status_submit",
    G_CALLBACK (twitux_entry_submit_cb), NULL);
  
  /* Actions */
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_end (GTK_BOX (entry), hbox, FALSE, FALSE, 10);
  
  entry->priv->counter = gtk_label_new ("");
  gtk_label_set_use_markup (GTK_LABEL (entry->priv->counter), TRUE);
  gtk_box_pack_end (GTK_BOX (hbox), entry->priv->counter, FALSE, FALSE, 0);
  
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (entry->priv->input));
  g_signal_connect (buffer, "changed",
    G_CALLBACK (twitux_entry_changed_cb), entry);
  
  /* Initial character count message */
  standard_msg = _("characters left");
  character_count =
      g_markup_printf_escaped ("<span size=\"small\"><b>%i</b> %s</span>",
                               MAX_CHARACTER_COUNT, standard_msg);
  gtk_label_set_markup (GTK_LABEL (entry->priv->counter), character_count);
  g_free (character_count);
}

void
twitux_entry_submit_cb (GtkWidget *widget,
                        gpointer   user_data)
{
  g_print ("TODO: User update it status\n");
}

void
twitux_entry_changed_cb (GtkTextBuffer *textbuffer,
                         gpointer       user_data)
{
  TwituxEntry *entry = TWITUX_ENTRY (user_data);
  const gchar *standard_msg;
  gchar *character_count;
  gint i, count; 

  i = gtk_text_buffer_get_char_count (textbuffer);

  count = MAX_CHARACTER_COUNT - i;
  standard_msg = _("characters left");

  if (i > MAX_CHARACTER_COUNT){
    character_count =
      g_markup_printf_escaped ("<span size=\"small\"><b><span foreground=\"red\">%i</span></b> %s</span>",
                               count, standard_msg);
  } else {
    character_count =
      g_markup_printf_escaped ("<span size=\"small\"><b>%i</b> %s</span>",
                               count, standard_msg);
  }
  
  gtk_label_set_markup (GTK_LABEL (entry->priv->counter), character_count);
  g_free (character_count);
}

GtkWidget*
twitux_entry_new (void)
{
  TwituxEntry *entry;

  entry = g_object_new (TWITUX_TYPE_ENTRY, NULL);

  return GTK_WIDGET (entry);
}
