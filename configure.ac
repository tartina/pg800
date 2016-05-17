#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pg800], [0.7], [guido.aulisi@gmail.com],, [http://pinguino.linkpc.net])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([main.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
LT_INIT
PKG_PROG_PKG_CONFIG
AX_CXX_COMPILE_STDCXX(11,,[mandatory])

# Checks for libraries.
PKG_CHECK_MODULES([gtkmm], [gtkmm-3.0 >= 3.8])
PKG_CHECK_MODULES([librtmidi], [librtmidi >= 2.1], [AC_DEFINE([HAVE_LIBRTMIDI], [1], [New librtmidi])],
	[PKG_CHECK_MODULES([rtmidi], [rtmidi >= 1.0], [AC_DEFINE([HAVE_RTMIDI], [1], [Old rtmidi])])]
)
PKG_CHECK_MODULES([libxml], [libxml++-2.6 >= 2.37])
BOOST_REQUIRE([1.53.0])
BOOST_CONVERSION
BOOST_STRING_ALGO

# Checks for header files.
old_CPPFLAGS=$CPPFLAGS
CPPFLAGS="${rtmidi_CFLAGS} ${libxml_CFLAGS} $CPPFLAGS"
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS(boost/lexical_cast.hpp boost/algorithm/string.hpp, [], [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS(RtMidi.h, [], [AC_MSG_ERROR(RtMidi.h not found)])
AC_CHECK_HEADERS(libxml++/libxml++.h, [], [AC_MSG_ERROR(libxml++.h not found)])
AC_LANG_POP([C++])
CPPFLAGS=$old_CPPFLAGS

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [enable debug]),
	AC_DEFINE([HAVE_DEBUG], [1], [Enable debugging code]))

AC_CONFIG_FILES([Makefile pg800.spec])
AC_OUTPUT
