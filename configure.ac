#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([pulsed-lattice-sim], [1.0], [http://github.com/ZJ/pulsed-lattice-sim/issues], [pulsed-lattice-sim])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

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
AC_CHECK_HEADERS([stdlib.h math.h fftw3.h])
# AC_CHECK_HEADER([fftw3.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 tests/Makefile
])
AC_OUTPUT
