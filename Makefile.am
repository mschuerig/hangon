SUBDIRS = po src tests doc

ACLOCAL_AMFLAGS = -I m4

man_page = $(PACKAGE).1
man_MANS = $(man_page)
CLEANFILES = $(man_page)

EXTRA_DIST = hangon.x m4/ChangeLog $(man_MANS)

# Create the man page from --help and --version output

common_dep = $(top_srcdir)/configure.ac
### TODO remove the explicit name
$(man_page): $(common_dep) $(srcdir)/hangon.x $(srcdir)/src/hangon.c

SUFFIXES = .x .1
.x.1:
	$(HELP2MAN) \
	 --no-info \
	 --include=$(srcdir)/$*.x \
	 --output=$@ src/$*
