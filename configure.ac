AC_INIT([twitux],
        [0.70],
        [http://github.com/danielm/twitux/issues],
        [twitux],
        [http://github.com/danielm/twitux/])

AC_PREREQ([2.64])

AC_COPYRIGHT([
  Copyright (C) 2012 Daniel Morales
])

#
# Hard dependencies
#
GLIB_REQUIRED=2.30.0
GTK_REQUIRED=3.0.0
REST_REQUIRED=0.7.12
LIBNOTIFY_REQUIRED=0.7.0
LIBSECRET_REQUIRED=0.5
LIBCANBERRA_GTK_REQUIRED=0.25

#
# Optional dependencies
# 
ENCHANT_REQUIRED=1.2.0
ISO_CODES_REQUIRED=0.35

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/twitux.c])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-dist-gzip dist-xz])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([enable])

#
# Programs
#
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AM_PATH_GLIB_2_0

LT_PREREQ([2.2])
LT_INIT

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
GLIB_GSETTINGS

#
# Gnome macros
#
GNOME_COMPILE_WARNINGS([maximum])
GNOME_CXX_WARNINGS([yes])
GNOME_DEBUG_CHECK
GNOME_MAINTAINER_MODE_DEFINES

#
# Required packages
#
PKG_CHECK_MODULES(TWITUX,
[
  dbus-glib-1
  glib-2.0 >= $GLIB_REQUIRED
  gio-2.0 >= $GLIB_REQUIRED
  gio-unix-2.0 >= $GLIB_REQUIRED
  gobject-2.0
  gsettings-desktop-schemas
  gtk+-3.0 >= $GTK_REQUIRED
  rest-0.7 >= $REST_REQUIRED
  libnotify >= $LIBNOTIFY_REQUIRED
  libcanberra-gtk3 >= $LIBCANBERRA_GTK_REQUIRED
  libsoup-2.4
])
#libsecret-1 >= $LIBSECRET_REQUIRED
AC_SUBST(TWITUX_CFLAGS)
AC_SUBST(TWITUX_LIBS)

# 
# Spellchecking (Enchant and ISO-Codes)
#
AC_ARG_ENABLE(spell,
              AS_HELP_STRING([--enable-spell=@<:@no/yes/auto@:>@],
                             [Enable spell checking]), ,
                             enable_spell=auto)

if test "x$enable_spell" != "xno"; then
   PKG_CHECK_MODULES(ENCHANT,
   [
      enchant >= $ENCHANT_REQUIRED,
      iso-codes >= $ISO_CODES_REQUIRED
   ], have_enchant="yes", have_enchant="no")

   if test "x$have_enchant" = "xyes"; then
      AC_MSG_CHECKING([whether iso-codes has iso-639 and iso-3166 domains])
      if $PKG_CONFIG --variable=domains iso-codes | grep 639 > /dev/null && \
         $PKG_CONFIG --variable=domains iso-codes | grep 3166 > /dev/null ; then
            AC_DEFINE_UNQUOTED(ISO_CODES_PREFIX, ["`$PKG_CONFIG --variable=prefix iso-codes`"], [ISO codes prefix])
            AC_DEFINE(HAVE_ENCHANT, [], [Define if you have libenchant])
            result=yes
      else
            result=no
            have_enchant="no"
      fi
      AC_MSG_RESULT([$result])
   fi
else
   have_enchant=no
fi

if test "x$enable_spell" = "xyes" -a "x$have_enchant" != "xyes"; then
   AC_MSG_ERROR([Could not find spell-checking dependencies:

$ENCHANT_PKG_ERRORS])
fi

AM_CONDITIONAL(HAVE_ENCHANT, test "x$have_enchant" = "xyes")

#
# Language Support
#
IT_PROG_INTLTOOL([0.50.0])

GETTEXT_PACKAGE=AC_PACKAGE_NAME
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[Gettext package name])
AM_GLIB_GNU_GETTEXT

#
# Generate Makefiles
#
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  data/Makefile
  data/twitux.desktop.in
  po/Makefile.in
])

AC_OUTPUT

echo
echo $PACKAGE $VERSION
echo
echo "Prefix:   $prefix"

echo "
Configure summary:

	Compiler....................:  ${CC}
	Compiler Flags..............:  ${CFLAGS} ${ERROR_CFLAGS}
	Linker Flags................:  ${LDFLAGS}
	Prefix......................:  ${prefix}

    Features:
	Spell checking (enchant)....:  ${have_enchant}
"
