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
