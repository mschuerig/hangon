AC_INIT([hangon],[0.1],[michael@schuerig.de])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_PROG_CC

gl_EARLY
gl_INIT

dnl This isn't really necessary, is it?
AC_PROG_RANLIB

# -Wextra enables additional checks not included in -Wall
# AM_INIT_AUTOMAKE doesn't accept -Wextra so it is added
# manually. See gcc(1).
#
# _FORTIFY_SOURCE adds some checking against buffer overflows.
# See feature_test_macros(7).
CFLAGS="$CFLAGS -Wextra -D_FORTIFY_SOURCE=2"

AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug], [enable debugging]),
              enable_debug="yes", [])
if test "x$enable_debug" = "xyes"; then
    # Add some heavy-weight pointer checking using mudflap
    # See gcc(1) and <http://gcc.gnu.org/wiki/Mudflap_Pointer_Debugging>.
    CPPFLAGS="$CPPFLAGS -DDEBUG"
    CFLAGS="$CFLAGS -O0 -g -fmudflap"
    LDFLAGS="$LDFLAGS -lmudflap"
else
    CPPFLAGS="$CPPFLAGS -DNDEBUG"
fi

# Use a libintl installed on the system.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/hangon.c])
AC_CONFIG_FILES([Makefile gnulib/lib/Makefile src/Makefile doc/Makefile po/Makefile.in])

AC_CONFIG_TESTDIR([tests], [tests:src])
AC_CONFIG_FILES([tests/Makefile])

# Define additional programs needed for hacking on this project,
# but not required for simply building it.
# autom4te: used for merging tests into a suite
AM_MISSING_PROG([AUTOM4TE], [autom4te])

if test "x$cross_compiling" = "xno"; then
  # help2man: used for deriving man pages from --help and --version output
  AM_MISSING_PROG([HELP2MAN], [help2man])
else
  HELP2MAN=:
fi

AC_OUTPUT
