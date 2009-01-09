AC_INIT([hangon],[0.1],[michael@schuerig.de])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_PROG_CC

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
    CFLAGS="$CFLAGS -O0 -g -DDEBUG -fmudflap -lmudflap"
else
    CFLAGS="$CFLAGS -DNDEBUG"
fi

# Use a libintl installed on the system.
AM_GNU_GETTEXT([external])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile po/Makefile.in])

AC_OUTPUT
