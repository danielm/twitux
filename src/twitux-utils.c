/* Twitux - Common functions
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

#include "config.h"
#include "twitux-utils.h"

#include <glib/gi18n.h>

void
twitux_utils_show_about (GtkWidget *parent)
{
  const gchar *authors[] = {
    "Daniel Morales <daniminas@gmail.com>",
    NULL
  };

  const gchar *translators = _("translator-credits");
  const gchar *copyright = "2012 Daniel Morales";

  gtk_show_about_dialog (GTK_WINDOW (parent),
                         "program-name", "Twitux",
                         "comments", _("Microblogging client for Gnome"),
                         "version", VERSION,
                         "license-type", GTK_LICENSE_GPL_2_0,
                         "authors", authors,
                         "copyright", copyright,
                         "translator-credits", translators,
                         NULL);
}

gchar *
twitux_utils_lookup_file (const gchar *filename)
{
  return g_build_filename (TWITUX_DATADIR, filename, NULL);
}
