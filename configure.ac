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

# Checks for libraries.
PKG_CHECK_MODULES([gtkmm], [gtkmm-3.0 >= 3.10])
PKG_CHECK_MODULES([rtmidi], [rtmidi >= 1.0])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_ARG_ENABLE([debug],
	      AS_HELP_STRING([--enable-debug], [enable debug]),
	      AC_DEFINE([HAVE_DEBUG], [1], [Enable debugging code]))

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
