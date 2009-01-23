AC_PREREQ(2.61)
AC_INIT([hangon],[0.1],[michael@schuerig.de])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_CONFIG_SRCDIR([src/hangon.c])
AC_CONFIG_HEADERS([config.h])

# Options
AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug], [enable debugging]),
              enable_debug="true")
AM_CONDITIONAL([DEBUG], [test x$enable_debug = xtrue])

# Checks for programs.
AC_PROG_CC

## gnulib needs to be initialized before any further checks.
gl_EARLY
gl_INIT

## Checks for libraries.
## would go here

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([locale.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([atexit dup2 setlocale])

# Checks for gettext.
## Use a libintl installed on the system.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])

# Testing
AC_CONFIG_TESTDIR([tests], [tests:src])

## Define additional programs needed for hacking on this project,
## but not required for simply building it.
## autom4te: used for merging tests into a suite
AM_MISSING_PROG([AUTOM4TE], [autom4te])

if test "x$cross_compiling" = "xno"; then
  # help2man: used for deriving man pages from --help and --version output
  AM_MISSING_PROG([HELP2MAN], [help2man])
else
  HELP2MAN=:
fi

## Generate Output
AC_CONFIG_FILES([Makefile gnulib/lib/Makefile src/Makefile doc/Makefile po/Makefile.in tests/Makefile])
AC_OUTPUT
