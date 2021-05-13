# Process this file with autoconf to produce a configure script.
# Written by Salonia Matteo on 13/05/2021

AC_PREREQ([2.69])

AC_INIT([csnake], [1.3], [saloniamatteo@pm.me])
AC_CONFIG_SRCDIR([src])

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([locale.h stdlib.h string.h ncurses.h getopt.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

# Custom parameter --with-docdir
AC_ARG_WITH([docdir],
	[AS_HELP_STRING([--with-docdir=DIR], [Where the documentation will be installed])],
	[docdir=$withval],
	[docdir="\${libdir}/doc"])

# Check if argument has been passed
if test "x$docdir" = "x"; then
	AC_SUBST([docdir], ["/usr/share/doc/csnake"])
else
	AC_SUBST([docdir], [$docdir])
fi

AM_INIT_AUTOMAKE([gnu subdir-objects])
AC_CONFIG_FILES([Makefile])
AC_PROG_CC

AC_OUTPUT
