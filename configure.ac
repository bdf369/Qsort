#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([Qsort], [0.1], [friedman.barry@gmail.com], [qsort],
        [http://github.com/bdf369/Qsort])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_SRCDIR([src/qsort.c])
AC_CONFIG_HEADERS([config.h])

AC_ARG_ENABLE([median-pivot],
              AC_HELP_STRING([--enable-median-pivot],
                             [Use median of first,mid,last as pivot]))

if test "${enable_median_pivot}" = "yes"; then
  AC_DEFINE([MEDIAN_PIVOT],1,
            [Use median of fist,mid,last as pivot instead of rand()])
fi

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

# Checks for library functions.
AC_CHECK_LIB([rt], [clock_gettime])

# We don't use malloc(0), so skip this check
#AC_FUNC_MALLOC

AC_CHECK_FUNCS([clock_gettime])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
