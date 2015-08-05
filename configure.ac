#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pulsed-lattice-sim], [1.0], [http://github.com/ZJ/pulsed-lattice-sim/issues], [pulsed-lattice-sim])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([m], [exp])
AC_CHECK_LIB([fftw3], [fftw_execute])

# Checks for header files.
AC_CHECK_HEADER([math.h])
AC_CHECK_HEADER([fftw3.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
