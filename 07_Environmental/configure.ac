#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([RHasher], [1.0], [https://github.com/Winking-maniac])
AC_CONFIG_SRCDIR([rhasher.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_INIT_AUTOMAKE

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lrhash':
AC_CHECK_LIB([rhash], [rhash_library_init], [], [
  AC_MSG_ERROR([Unable to find librhash])
])

AC_ARG_WITH([readline], 
  [AS_HELP_STRING([--without-readline], [build without using readline])], 
  [],
  [AC_CHECK_LIB([readline], [readline],
    [AC_SUBST([rhasher_LDADD], ["-lreadline"])
     AC_DEFINE([HAVE_LIBREADLINE], [1], [desc])])]
)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
