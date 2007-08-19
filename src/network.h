#if !defined TWITUX_NETWORK_H
#define TWITUX_NETWORK_H
/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007 - Alvaro Daniel Morales - <daniel@suruware.com>
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

#include "main.h"
#include "twitter.h"

void tt_network_load ( TwiTux *twitter );

void tt_network_get_timeline ( TwiTux *twitter, const char *url, gboolean restet_lastid  );
 
void tt_network_get_image ( TwiTux *twitter, TwiTuxStatus *status );

void tt_network_login ( TwiTux *twitter );

void tt_network_direct_messages ( TwiTux *twitter );

void tt_network_post_status ( TwiTux *twitter, gchar *text );

void tt_network_send_message ( TwiTux *twitter, gchar *friend, gchar *text );

void tt_network_tinyze ( TwiTuxTinyzer *tinyzer, const char *url );

// Funciones del timeout que actualiza el timeline actual
gboolean tt_timeout_remove ( TwiTux *twitter );

gboolean tt_timeout_start ( TwiTux *twitter );

#endif