
info_TEXINFOS = tutorial.texi

tutorial.pdf: stamp-excerpts

stamp-excerpts: $(top_srcdir)/src/hangon.c
	$(top_srcdir)/tools/excerpt -o -d excerpts $(top_srcdir)/src/hangon.c
	touch stamp-excerpts

clean-local:
	-rm -f stamp-excerpts
	-rm -rf excerpts
