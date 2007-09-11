/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007 - Daniel Morales <daniminas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include <string.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <libgnome/gnome-util.h>
#include <libsoup/soup.h>

#include <libtwitux/twitux-debug.h>
#include <libtwitux/twitux-conf.h>

#include "twitux.h"
#include "twitux-network.h"
#include "twitux-parser.h"
#include "twitux-app.h"

#define DEBUG_DOMAIN_SETUP	"Network" 
#define DEBUG_QUIT

typedef struct {
	gchar        *src;
	GtkTreeIter  iter;
	GtkListStore *store;
} TwituxImage;

static void network_get_data		(const gchar *url,
				 	 SoupMessageCallbackFn callback,
				 	 gpointer data);

static void network_post_data		(const gchar *url,
					 gchar *formdata,
					 SoupMessageCallbackFn callback);

static gboolean	network_check_http 	(gint status_code);

static gchar *network_save_data		(SoupMessage *msg,
					 const char *file);

static void network_parser_free_lists	();

/* libsoup callbacks */
static void network_cb_on_login		(SoupMessage *msg,
					 gpointer user_data);
static void network_cb_on_post		(SoupMessage *msg,
					 gpointer user_data);
static void network_cb_on_message	(SoupMessage *msg,
					 gpointer user_data);
static void network_cb_on_timeline	(SoupMessage *msg,
					 gpointer user_data);
static void network_cb_on_users	(SoupMessage *msg,
					 gpointer user_data);
static void network_cb_on_image		(SoupMessage *msg,
					 gpointer user_data);
static void network_cb_on_add		(SoupMessage *msg,
					 gpointer user_data);
static void network_cb_on_del		(SoupMessage *msg,
					 gpointer user_data);
static void network_cb_on_auth		(SoupSession *session,
					 SoupMessage *msg,
					 const char *auth_type,
					 const char *auth_realm,
					 char **username,
					 char **password,
					 gpointer data);

/* Autoreload timeout functions */
static gboolean 	network_timeout		(gpointer user_data);
static void			network_timeout_new	(void);

static SoupSession			*soup_connection = NULL;
static GList				*user_friends = NULL;
static GList				*user_followers = NULL;
static gboolean				processing = FALSE;
static gchar				*current_timeline = NULL;
static guint				timeout_id;

/* This function must be called at startup */
void
twitux_network_new ()
{
	TwituxConf	*conf;
	gboolean	check_proxy = FALSE;
 
	/* Close previous networking */
	if (soup_connection) {
		twitux_network_close ();
	}

	/* Async connections */
	soup_connection = soup_session_async_new_with_options (
					SOUP_SESSION_MAX_CONNS, 8,
					NULL);

	twitux_debug (DEBUG_DOMAIN_SETUP, "Libsoup (re)started");

	/* Set the proxy, if configuration is set */
	conf = twitux_conf_get ();
	twitux_conf_get_bool (conf, TWITUX_PROXY_USE, &check_proxy);

	if(check_proxy) {
		gchar *server, *proxy_uri;
		gint port;

		/* Get proxy */
		twitux_conf_get_string (conf, TWITUX_PROXY_HOST, &server);
		twitux_conf_get_int (conf, TWITUX_PROXY_PORT, &port);

		if (server && server[0]) {
			SoupUri *suri;

			check_proxy = FALSE;
			twitux_conf_get_bool (conf, TWITUX_PROXY_USE_AUTH,
					      &check_proxy);

			/* Get proxy auth data */
			if (check_proxy){
				char *user, *password;

				twitux_conf_get_string (conf, TWITUX_PROXY_USER,
							&user);
				twitux_conf_get_string (conf, TWITUX_PROXY_PASS,
							&password);

				proxy_uri = g_strdup_printf (
						"http://%s:%s@%s:%d",
						 user, password, server, port);

				g_free (user);
				g_free (password);
			} else {
				proxy_uri = g_strdup_printf ("http://%s:%d", 
							      server, port);
			}

			twitux_debug (DEBUG_DOMAIN_SETUP, "Proxy uri: %s",
					proxy_uri);

			/* Setup proxy info */
			suri = soup_uri_new (proxy_uri);
			g_object_set (G_OBJECT (soup_connection),
						 SOUP_SESSION_PROXY_URI, suri,
						 NULL);

			soup_uri_free (suri);
			g_free (server);
			g_free (proxy_uri);
		}
	}
}


/* Cancels requests, and unref libsoup. */
void
twitux_network_close ()
{
	/* Close all connections */
	twitux_network_stop ();

	network_parser_free_lists ();

	g_object_unref (soup_connection);

	if (current_timeline) g_free (current_timeline);

	twitux_debug (DEBUG_DOMAIN_SETUP, "Libsoup closed");
}


/* Cancels all pending requests in session. */
void
twitux_network_stop	()
{
	twitux_debug (DEBUG_DOMAIN_SETUP,"Cancelled all connections");

	soup_session_abort (soup_connection);
}


/* Login in Twitter */
void
twitux_network_login ()
{
	twitux_debug (DEBUG_DOMAIN_SETUP, "Begin login.. ");

	twitux_app_set_statusbar_msg (_("Connecting..."));

	/* HTTP Basic Authentication */
	g_signal_connect (soup_connection, "authenticate", 
			  G_CALLBACK (network_cb_on_auth), NULL);
	g_signal_connect (soup_connection, "reauthenticate",
			  G_CALLBACK (network_cb_on_auth), NULL);

	/* Verify cedentials */
	network_get_data (TWITUX_API_LOGIN, network_cb_on_login, NULL);
}


/* Logout current user */
void twitux_network_logout ()
{
	twitux_network_new ();
	
	twitux_debug (DEBUG_DOMAIN_SETUP, "Logout");
}


/* Post a new tweet - text must be Url encoded */
void
twitux_network_post_status (const gchar *text)
{
	gchar *formdata;

	formdata = g_strdup_printf ("status=%s", text);

	network_post_data (TWITUX_API_POST_STATUS,
			   formdata,
			   network_cb_on_post);
}


/* Send a direct message to a follower - text must be Url encoded  */
void
twitux_network_send_message (const gchar *friend,
			     const gchar *text)
{
	gchar *formdata;

	formdata = g_strdup_printf ( "user=%s&text=%s", friend, text);
	
	network_post_data (TWITUX_API_SEND_MESSAGE,
			   formdata,
			   network_cb_on_message);
}

void
twitux_network_refresh ()
{
	if (!current_timeline || processing) return;

	/* UI */
	twitux_app_set_statusbar_msg (_("Loading timeline..."));

	processing = TRUE;
	network_get_data (current_timeline, network_cb_on_timeline, NULL);
}

/* Get and parse a timeline */
void
twitux_network_get_timeline (const gchar *url_timeline)
{
	if (processing) return;

	/* UI */
	twitux_app_set_statusbar_msg (_("Loading timeline..."));

	processing = TRUE;
	network_get_data (url_timeline, network_cb_on_timeline, g_strdup(url_timeline));
}

/* Get a user timeline */
void
twitux_network_get_user (const gchar *username)
{
	gchar *user_timeline;
	gchar *user_id;

	if (!username){
		TwituxConf *conf;
		conf = twitux_conf_get ();
		twitux_conf_get_string (conf,
			TWITUX_PREFS_AUTH_USER_ID,
			&user_id);
	} else {
		user_id = g_strdup (username);
	}

	if(!G_STR_EMPTY (user_id)) {
		user_timeline = g_strdup_printf (TWITUX_API_TIMELINE_USER,
							user_id);
	
		twitux_network_get_timeline (user_timeline);
		g_free (user_timeline);
	}

	if (user_id) g_free (user_id);
}

/* Get authenticating user's friends(following)
 * Returns:
 * 		NULL: Friends will be fetched
 * 		GList: The list of friends (fetched previously)
 */
GList *
twitux_network_get_friends ()
{
	gboolean friends = TRUE;

	if(user_friends) return user_friends;
	
	network_get_data (TWITUX_API_FOLLOWING,
			network_cb_on_users, GINT_TO_POINTER(friends));
	
	return NULL;
}


/* Get the authenticating user's followers
 * Returns:
 * 		NULL: Followers will be fetched
 * 		GList: The list of friends (fetched previously)
 */
GList *
twitux_network_get_followers ()
{
	gboolean friends = FALSE;

	if(user_followers) return user_followers;

	network_get_data (TWITUX_API_FOLLOWERS,
			network_cb_on_users, GINT_TO_POINTER(friends));

	return NULL;
}


/* Get an image from servers */
void
twitux_network_get_image (const gchar *url_image,
						  const gchar *username,
						  GtkTreeIter iter,
						  GtkListStore *store)
{
	gchar	*images_dir;
	gchar	*image_file;
	gchar	**split_url;

	TwituxImage *image;

	/* Build image file */
	split_url = g_strsplit (url_image, "?", 2);
	images_dir = gnome_util_home_file (TWITUX_CACHE_IMAGES);
	image_file = g_strconcat (images_dir,
				  G_DIR_SEPARATOR_S,
				  username,
				  "-",
				  split_url[1],
				  NULL);

	g_strfreev (split_url);
	g_free (images_dir);

	/* check if image already exists */
	if(g_file_test (image_file, G_FILE_TEST_EXISTS | 
					G_FILE_TEST_IS_REGULAR)){
		twitux_debug (DEBUG_DOMAIN_SETUP,
				"Loading image from cache: %s", image_file);
		
		/* Set image from file here */
		twitux_app_set_image (image_file, store, iter);

		g_free (image_file);
		return;
	}

	image = g_new0 (TwituxImage, 1);
	image->src = image_file;
	image->iter = iter;
	image->store = store;

	/* Note: 'image' will be freed in 'network_cb_on_image' */
	network_get_data (url_image, network_cb_on_image, image);
}



/* Add a user to follow */
void
twitux_network_add_user (const gchar *username)
{
	gchar *url;
	
	if (!username) return;
	
	url = g_strdup_printf (TWITUX_API_FOLLOWING_ADD, username);

	network_get_data (url, network_cb_on_add, NULL);

	g_free (url);
}


/* Add a user to follow */
void
twitux_network_del_user (TwituxUser *user)
{
	gchar *url;
	
	if (!user || !user->screen_name) return;
	
	url = g_strdup_printf (TWITUX_API_FOLLOWING_DEL, user->screen_name);

	network_get_data (url, network_cb_on_del, user);

	g_free (url);
}


/* Get data from net */
static void
network_get_data (const gchar *url,
		  SoupMessageCallbackFn callback,
		  gpointer data)
{
	SoupMessage *msg;

	twitux_debug (DEBUG_DOMAIN_SETUP, "Get: %s",url);

	msg = soup_message_new ( "GET", url );

	soup_session_queue_message (soup_connection, msg, callback, data);
}


/* Private: Post data to net */
static void
network_post_data (const gchar *url,
		  gchar *formdata,
		  SoupMessageCallbackFn callback)
{
	SoupMessage *msg;

	twitux_debug (DEBUG_DOMAIN_SETUP, "Post: %s",url);

	msg = soup_message_new ("POST", url);
	
	soup_message_add_header (msg->request_headers,
			"X-Twitter-Client", PACKAGE_NAME);
	soup_message_add_header (msg->request_headers,
			"X-Twitter-Client-Version", PACKAGE_VERSION);
	soup_message_add_header (msg->request_headers,
			"X-Twitter-Client-URL", TWITUX_HEADER_URL);

	soup_message_set_request (msg, 
			"application/x-www-form-urlencoded",
			SOUP_BUFFER_USER_OWNED,
			formdata,
			strlen (formdata));

	soup_session_queue_message (soup_connection, msg, callback, NULL);
}


/* Check HTTP response code */
static gboolean
network_check_http (gint status_code)
{
	if (status_code == 401) {
		twitux_app_set_statusbar_msg (_("Access denied."));

	} else if (SOUP_STATUS_IS_CLIENT_ERROR (status_code)) {
		twitux_app_set_statusbar_msg (_("HTTP communication error."));

	} else if(SOUP_STATUS_IS_SERVER_ERROR (status_code)) {
		twitux_app_set_statusbar_msg (_("Internal server error."));

	} else if (!SOUP_STATUS_IS_SUCCESSFUL (status_code)) {
		twitux_app_set_statusbar_msg (_("Stopped"));

	} else {
		return TRUE;
	}
	
	return FALSE;
}


/* Save data to disk */
static gchar *
network_save_data (SoupMessage *msg,
                   const char *file)
{
	gchar *tmp_file = NULL;

	/* File dir */
	tmp_file = gnome_util_home_file (file);

	twitux_debug (DEBUG_DOMAIN_SETUP, "Saving data to: %s",file);

	/* Save */
	if (g_file_set_contents (tmp_file,
				 msg->response.body,
				 msg->response.length,
				 NULL)){
		return tmp_file;
	}
	
	twitux_app_set_statusbar_msg (_("Failed to save cache file."));

	g_free (tmp_file);
	return NULL;
}


/* Callback to free every element on a User list */
static void
network_free_user_list_each (gpointer user,
							 gpointer data)
{
	TwituxUser *usr;

	if (!user) return;

	usr = (TwituxUser *)user;
	parser_free_user (user);
}


/* Free a list of Users */
static void
network_parser_free_lists ()
{
	if (user_friends) {
		g_list_foreach (user_friends, network_free_user_list_each, NULL);
		g_list_free (user_friends);
		user_friends = NULL;
		twitux_debug (DEBUG_DOMAIN_SETUP,
			"Friends freed");
	}
	if (user_followers) {
		g_list_foreach (user_followers, network_free_user_list_each, NULL);
		g_list_free (user_followers);
		user_followers = NULL;
		twitux_debug (DEBUG_DOMAIN_SETUP,
			"Followers freed");
	}
}



/* HTTP Basic Authentication */
static void
network_cb_on_auth (SoupSession *session,
		    SoupMessage *msg,
		    const char *auth_type,
		    const char *auth_realm,
		    char **username,
		    char **password,
		    gpointer data )
{
	TwituxConf *conf;
	gchar      *user_id;
	gchar      *user_passwd;

	conf = twitux_conf_get ();

	twitux_conf_get_string (conf,
				TWITUX_PREFS_AUTH_USER_ID,
				&user_id);
	twitux_conf_get_string (conf,
				TWITUX_PREFS_AUTH_PASSWORD,
				&user_passwd);

	/* verify that user id & password has been set */
	if (!(G_STR_EMPTY (user_id)) && !(G_STR_EMPTY (user_passwd))){
		*username = g_strdup (user_id);
		*password = g_strdup (user_passwd);
		return;
	}
	
	if (user_id) g_free (user_id);
	if (user_passwd) g_free (user_passwd);
}


/* On verify credentials */
static void
network_cb_on_login (SoupMessage *msg,
		     gpointer user_data)
{
	twitux_debug (DEBUG_DOMAIN_SETUP,
			"Login response: %i",msg->status_code);

	if (network_check_http (msg->status_code)) {
		twitux_app_set_statusbar_msg (_("Connected to Twitter"));
		twitux_app_state_on_connection (TRUE);
		twitux_app_set_friends (NULL);
		return;
	}

	twitux_app_state_on_connection (FALSE);
}


/* On post a tweet */
static void
network_cb_on_post (SoupMessage *msg,
		    gpointer user_data)
{
	/* Free buffer memory
	 * Review: we need to free something more? */
	g_free (msg->request.body);
	
	if (network_check_http (msg->status_code)) {
		twitux_app_set_statusbar_msg (_("Status Sent"));
	}
	
	twitux_debug (DEBUG_DOMAIN_SETUP,
		"Tweet response: %i",msg->status_code);
}


/* On send a direct message */
static void
network_cb_on_message (SoupMessage *msg,
		       gpointer user_data)
{
	/* Free buffer memory
	 * Review: we need to free something more? */
	g_free (msg->request.body);

	if (network_check_http (msg->status_code)) {
		twitux_app_set_statusbar_msg (_("Message Sent"));
	}

	twitux_debug (DEBUG_DOMAIN_SETUP,
		"Message response: %i",msg->status_code);
}


/* On get a timeline */
static void
network_cb_on_timeline (SoupMessage *msg,
			gpointer user_data)
{
	gchar *file;
	GtkListStore *store;
	gchar *new_timeline = NULL;
	
	if (user_data){
		new_timeline = (gchar *)user_data;
	}

	twitux_debug (DEBUG_DOMAIN_SETUP,
		"Timeline response: %i",msg->status_code);

	processing = FALSE;

	/* Timeout */
	network_timeout_new ();

	/* Check response */
	if (!network_check_http (msg->status_code)){
		if (new_timeline) g_free (new_timeline);
		return;
	}

	/* Save */
	file = network_save_data (msg, TWITUX_CACHE_FILE);
	if (!file) return;

	twitux_debug (DEBUG_DOMAIN_SETUP, "Parsing timeline: %s",file);

	/* Parse and set ListStore */
	store = twitux_parser_timeline (file);

	if (store) {
		twitux_app_set_liststore (store);
		twitux_app_set_statusbar_msg (_("Timeline Loaded"));
		if (new_timeline){
			if (current_timeline) g_free (current_timeline);
			current_timeline = g_strdup (new_timeline);
		}
	} else {
		twitux_app_set_statusbar_msg (_("Timeline Parser Error."));
	}

	if (new_timeline) g_free (new_timeline);
	g_free (file);
}


/* On get user followers */
static void
network_cb_on_users (SoupMessage *msg,
			 gpointer user_data)
{
	gboolean friends = GPOINTER_TO_INT(user_data);
	GList *users;
	gchar *file;
		
	twitux_debug (DEBUG_DOMAIN_SETUP,
		"Users response: %i",msg->status_code);
	
	/* Check response */
	if (!network_check_http (msg->status_code)) return;
	
	/* Save */
	file = network_save_data (msg, TWITUX_CACHE_USERS);
	if (!file) return;

	/* parse user list */
	twitux_debug (DEBUG_DOMAIN_SETUP, "Parsing user list: %s",file);

	users = twitux_parser_users_list (file);

	/* check if it ok, and if it is a followers or following list */
	if (users && friends){
		/* Friends timeline retrived */
		user_friends = users;
		twitux_app_set_friends (users);
	} else if (users){
		/* TODO: some UI here. Followers timeline retrived */
	} else {
		twitux_app_set_statusbar_msg (_("Users parser error."));
	}

	g_free (file);
}


/* On get a image */
static void
network_cb_on_image (SoupMessage *msg,
		     gpointer user_data)
{
	TwituxImage *image = (TwituxImage *)user_data;

	twitux_debug (DEBUG_DOMAIN_SETUP,
		"Image response: %i", msg->status_code);

	/* check response */
	if (network_check_http (msg->status_code)) {
		/* Save image data */
		if (g_file_set_contents (image->src,
				 msg->response.body,
				 msg->response.length,
				 NULL)){
			/* Set image from file here (image_file) */
			twitux_app_set_image (image->src, image->store, image->iter);
		}
	}

	g_free (image->src);
	g_free (image);
}


/* On add a user */
static void
network_cb_on_add (SoupMessage *msg,
		     gpointer user_data)
{
	TwituxUser *user;
	gchar *file;

	twitux_debug (DEBUG_DOMAIN_SETUP,
		"Add user response: %i", msg->status_code);
	
	/* Check response */
	if (!network_check_http (msg->status_code)) return;

	/* Save */
	file = network_save_data (msg, TWITUX_CACHE_USERS);
	if (!file) return;

	/* parse new user */
	twitux_debug (DEBUG_DOMAIN_SETUP, "Parsing new user: %s",file);

	user = twitux_parser_single_user (file);

	if (user) {
		user_friends = g_list_append ( user_friends, user );
		twitux_app_set_statusbar_msg (_("Friend Added"));
	} else {
		twitux_app_set_statusbar_msg (_("Failed to add user"));
	}
	
	g_free (file);
}


/* On remove a user */
static void
network_cb_on_del (SoupMessage *msg,
		     gpointer user_data)
{
	twitux_debug (DEBUG_DOMAIN_SETUP,
		"Delete user response: %i", msg->status_code);

	if (network_check_http (msg->status_code)) {		
		twitux_app_set_statusbar_msg (_("Friend Removed"));
	} else {
		twitux_app_set_statusbar_msg (_("Failed to remove user"));
	}
	
	if (user_data) {
		TwituxUser *user = (TwituxUser *)user_data;
		user_friends = g_list_remove (user_friends, user);
		parser_free_user (user);
	}
}

static void
network_timeout_new ()
{
	gboolean reload;

	twitux_conf_get_bool (twitux_conf_get (),
						  TWITUX_PREFS_TWEETS_RELOAD_TIMELINES,
						  &reload);

	if (timeout_id > 0){
		twitux_debug (DEBUG_DOMAIN_SETUP,
					  "Stopping timeout id: %i", timeout_id);
		g_source_remove (timeout_id);
		timeout_id = 0;
	}

	if (reload) {
		timeout_id = g_timeout_add (TWITUX_TIMEOUT, network_timeout, NULL);

		twitux_debug (DEBUG_DOMAIN_SETUP,
					  "Starting timeout id: %i", timeout_id);
	}
}

static gboolean
network_timeout (gpointer user_data)
{
	if (!current_timeline || processing) return FALSE;

	/* UI */
	twitux_app_set_statusbar_msg (_("Reloading timeline..."));

	twitux_debug (DEBUG_DOMAIN_SETUP,
					  "Auto reloading. Timeout: %i", timeout_id);

	processing = TRUE;
	network_get_data (current_timeline, network_cb_on_timeline, NULL);

	timeout_id = 0;

	return FALSE;
}
