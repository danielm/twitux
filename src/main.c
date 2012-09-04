/* Twitux - Main file
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
 
#include <config.h>
#include <gtk/gtk.h>

#include "twitux-app.h"

#include <glib/gi18n.h>

int
main (int   argc,
      char *argv[])
{
  TwituxApp *app;
  int retval;

  bindtextdomain (GETTEXT_PACKAGE, TWITUX_LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  app = twitux_app_new ();
  retval = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return retval;
}
