bin_PROGRAMS = hangon
hangon_SOURCES = hangon.c

hangon_LDADD = @LIBINTL@ ../gnulib/lib/libgnu.a

localedir = $(datadir)/locale
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)/gnulib/lib -I$(top_builddir)/gnulib/lib
