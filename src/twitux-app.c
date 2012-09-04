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
 
#include "config.h"
#include "twitux-app.h"
#include "twitux-window.h"

#include <glib/gi18n.h>

#define APP_SERVICE_NAME "org.gnome.twitux.AppService"

struct _TwituxAppPrivate
{
  GtkWidget *main_window;
};

static void twitux_app_startup       (GApplication *app);
static void twitux_app_activate      (GApplication *application);
static void twitux_app_init          (TwituxApp *self);
static void twitux_app_finalize      (GObject *object);
static void twitux_app_class_init    (TwituxAppClass *klass);

static void twitux_app_init_app_menu (GApplication *application);

static void action_about (GSimpleAction *action,
                          GVariant      *parameter,
                          gpointer       user_data);
static void action_help  (GSimpleAction *action,
                          GVariant      *parameter,
                          gpointer       user_data);
static void action_quit  (GSimpleAction *action,
                          GVariant      *parameter,
                          gpointer       user_data);
           

G_DEFINE_TYPE (TwituxApp, twitux_app, GTK_TYPE_APPLICATION);

static GActionEntry app_entries[] = {
  { "about", action_about, NULL, NULL, NULL },
  { "help", action_help, NULL, NULL, NULL },
  { "quit", action_quit, NULL, NULL, NULL },
};

static void
twitux_app_activate (GApplication *application)
{
  TwituxAppPrivate *priv;

  priv = TWITUX_APP (application)->priv;

  if (priv->main_window != NULL)
  {
    gtk_window_present (GTK_WINDOW (priv->main_window));
  }
  else
  {
    priv->main_window = twitux_window_new (TWITUX_APP (application));
    gtk_window_set_title (GTK_WINDOW (priv->main_window), _("Twitux"));

    gtk_widget_show_all (priv->main_window);
  }
}

static void
twitux_app_init (TwituxApp *self)
{
  self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                            TWITUX_TYPE_APP,
                                            TwituxAppPrivate);
}

static void
twitux_app_finalize (GObject *object)
{
  G_OBJECT_CLASS (twitux_app_parent_class)->finalize (object);
}

static void
twitux_app_class_init (TwituxAppClass *klass)
{
  G_APPLICATION_CLASS (klass)->activate = twitux_app_activate;
  G_APPLICATION_CLASS (klass)->startup = twitux_app_startup;

  G_OBJECT_CLASS (klass)->finalize = twitux_app_finalize;

  g_type_class_add_private ((gpointer) klass, sizeof(TwituxAppPrivate));
}

static void
twitux_app_startup (GApplication *application)
{
  TwituxAppPrivate *priv;

  priv = TWITUX_APP (application)->priv;

  G_APPLICATION_CLASS (twitux_app_parent_class)->startup (application);

  twitux_app_init_app_menu (application);

  /* TODO: Setup status icon if shell isn't running */
}

static void
twitux_app_init_app_menu (GApplication *application)
{
  GtkBuilder *builder;
  GError *error = NULL;

  g_action_map_add_action_entries (G_ACTION_MAP (application),
                                   app_entries, G_N_ELEMENTS (app_entries),
                                   application);

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, TWITUX_DATADIR "/twitux-app-menu.xml", &error);

  if (error == NULL) {
    gtk_application_set_app_menu (GTK_APPLICATION (application),
                                  G_MENU_MODEL (gtk_builder_get_object (builder,
                                  "app-menu")));
  } else {
    g_critical ("Unable to add the application menu: %s\n", error->message);
    g_error_free (error);
  }
  
  g_object_unref (builder);
}

static void
action_about (GSimpleAction *action,
              GVariant      *parameter,
              gpointer       user_data)
{
  TwituxApp *application = TWITUX_APP (user_data);

  const gchar *authors[] = {
    "Daniel Morales <daniminas@gmail.com>",
    NULL
  };

  const gchar *translators = _("translator-credits");
  const gchar *copyright = "2012 Daniel Morales";

  gtk_show_about_dialog (GTK_WINDOW (application->priv->main_window),
                         "program-name", "Twitux",
                         "comments", _("Microblogging client for Gnome"),
                         "version", VERSION,
                         "license-type", GTK_LICENSE_GPL_2_0,
                         "authors", authors,
                         "copyright", copyright,
                         "translator-credits", translators,
                         NULL);
}

static void
action_help (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  GApplication *application = G_APPLICATION (user_data);

  /* TODO: Show help viewer */
}


static void
action_quit (GSimpleAction *action,
             GVariant      *parameter,
             gpointer       user_data)
{
  GApplication *application = G_APPLICATION (user_data);

  g_application_quit (application);
}

TwituxApp *
twitux_app_new (void)
{
  g_type_init ();

  g_set_application_name ("Twitux");

  return g_object_new (twitux_app_get_type (),
                       "application-id", APP_SERVICE_NAME,
                       NULL);
}
