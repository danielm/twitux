desktopdir = $(datadir)/applications
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
desktop_in_files = twitux.desktop.in
@INTLTOOL_DESKTOP_RULE@

gsettings_SCHEMAS = org.gnome.Twitux.gschema.xml
@GSETTINGS_RULES@

# We need to compile schemas at make time
# to run from source tree
gschemas.compiled: $(gsettings_SCHEMAS:.xml=.valid)
	$(AM_V_GEN) $(GLIB_COMPILE_SCHEMAS) --targetdir=. .

all-local: gschemas.compiled

uidir = $(pkgdatadir)
ui_DATA =				\
  twitux-app-menu.xml	\
  twitux-anonymous.png

EXTRA_DIST= 			\
  twitux.desktop		\
  $(desktop_in_files)	\
  $(gsettings_SCHEMAS)

CLEANFILES = 			\
  $(desktop_DATA)		\
  gschemas.compiled
