bin_PROGRAMS = hangon
hangon_SOURCES = hangon.c

hangon_LDADD = @LIBINTL@ ../gnulib/lib/libgnu.a

localedir = $(datadir)/locale

AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" \
  -I$(top_srcdir)/gnulib/lib \
  -I$(top_builddir)/gnulib/lib \
  -D_FORTIFY_SOURCE=2
# _FORTIFY_SOURCE adds some checking against buffer overflows.
# See feature_test_macros(7).

AM_CFLAGS = -std=c99 \
  -Wall -Wextra \
  -Wwrite-strings \
  -Wunused -Wno-unused-parameter \
  -Werror

if DEBUG
    # Add some heavy-weight pointer checking using mudflap
    # See gcc(1) and <http://gcc.gnu.org/wiki/Mudflap_Pointer_Debugging>.
    AM_CPPFLAGS	+= -DDEBUG
    AM_CFLAGS   += -O0 -g -fmudflap
    AM_LDFLAGS  = -lmudflap
else
    AM_CPPFLAGS += -DNDEBUG
endif
