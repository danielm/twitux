desktopdir = $(datadir)/applications
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
desktop_in_files = twitux.desktop.in

@INTLTOOL_DESKTOP_RULE@

uidir = $(pkgdatadir)
ui_DATA = twitux-app-menu.xml

EXTRA_DIST= 			\
  twitux.desktop		\
  $(desktop_in_files)

CLEANFILES = 			\
  $(desktop_DATA)
