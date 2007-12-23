/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2004-2007 Imendio AB
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
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Martyn Russell <martyn@imendio.com>
 *          Richard Hult <richard@imendio.com>
 *			Brian Pepple <bpepple@fedoraproject.org>
 */

#ifndef __TWITUX_SPELL_DIALOG_H__
#define __TWITUX_SPELL_DIALOG_H__

G_BEGIN_DECLS

void twitux_spell_dialog_show (GtkWidget   *window,
							   GtkTextIter  start,
							   GtkTextIter  end,
							   const gchar *word);

G_END_DECLS

#endif /* __TWITUX_SPELL_DIALOG_H__ */
