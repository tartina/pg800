#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pg800], [0.1], [guido.aulisi@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([main.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
PKG_PROG_PKG_CONFIG

# Checks for libraries.
PKG_CHECK_MODULES([gtkmm], [gtkmm-3.0 >= 3.8])
PKG_CHECK_MODULES([rtmidi], [rtmidi >= 1.0])
PKG_CHECK_MODULES([libxml], [libxml++-2.6 >= 2.37])
AC_LANG_PUSH([C++])
AC_SEARCH_LIBS([main], [boost_filesystem])
AC_LANG_POP([C++])

# Checks for header files.
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS(boost/lexical_cast.hpp boost/filesystem.hpp, [], [AC_MSG_ERROR(You need the Boost libraries.)])
AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [enable debug]),
	AC_DEFINE([HAVE_DEBUG], [1], [Enable debugging code]))

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
