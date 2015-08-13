#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([pulsed-lattice-sim], [1.0], [http://github.com/ZJ/pulsed-lattice-sim/issues], [pulsed-lattice-sim])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror foreign color-tests])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR
AS_CASE(AS_ESCAPE(["$HOST"],[""]),[*mingw32*],AS_ECHO(["AS_ESCAPE(["$HOST"],[""])"]),AS_ECHO(["No $HOST"]))
AM_CONDITIONAL([MINGWHOST], [[[ $HOST == *mingw* ]]])
LT_INIT([win32-dll])

# Check fo Check for Unit Testing
PKG_CHECK_MODULES([CHECK], [check >= 0.9.0],[have_check="yes"],
  AC_MSG_WARN([Check not found; cannot run unit tests!])
  [have_check="no"]
)
AM_CONDITIONAL(HAVE_CHECK, test x"$have_check" = "xyes")

# Checks for libraries.
AC_CHECK_LIB([m], [exp])
AC_CHECK_LIB([fftw3], [fftw_execute])

# Checks for header files.
AC_CHECK_HEADER([stdlib.h])
AC_CHECK_HEADERS([complex.h fftw3.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 tests/Makefile
])
AC_OUTPUT
