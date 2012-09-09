AM_CPPFLAGS =										\
	-DPREFIX=\""$(prefix)"\"						\
	-DLIBDIR=\""$(libdir)"\"						\
	-DDATADIR=\""$(datadir)"\"						\
	-DTWITUX_DATADIR=\""$(pkgdatadir)"\"			\
	-DTWITUX_LOCALEDIR=\""$(datadir)/locale"\"		\
	-DG_DISABLE_DEPRECATED							\
	-DGTK_DISABLE_DEPRECATED						\
	-DG_LOG_DOMAIN=\"TWITUX\"						\
	-I$(top_srcdir)									\
	-I$(top_builddir)								\
	$(ENCHANT_CFLAGS) 								\
	$(TWITUX_CFLAGS)

bin_PROGRAMS = twitux

twitux_SOURCES =									\
	main.c											\
	twitux-app.c			twitux-app.h			\
	twitux-utils.c			twitux-utils.h			\
	twitux-window.c			twitux-window.h			\
	twitux-viewer.c			twitux-viewer.h			\
	twitux-viewer-tile.c	twitux-viewer-tile.h

twitux_LDADD =										\
	$(TWITUX_LIBS)
