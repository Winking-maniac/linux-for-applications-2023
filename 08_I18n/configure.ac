#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([guess.c])
AC_CONFIG_HEADERS([config.h])


# Checks for programs.
AC_PROG_CC
AM_INIT_AUTOMAKE

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lrhash':
AC_CHECK_LIB([rhash], [main])

# Checks for header files.
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
