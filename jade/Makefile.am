## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = check-news

## FIXME groveoa is missing, since an .am with only EXTRA_DIST doesn't work.
SUBDIRS = jadedoc dsssl grove spgrove style jade intl po

pkginclude_HEADERS = config.h

docdir = $(prefix)/doc
pkgdocdir = $(docdir)/@PACKAGE@

pkgdoc_DATA = NEWS COPYING AUTHORS ChangeLog

BUILT_SOURCES = config.h msggen.pl

## Note that we include the windows-only aux files.
## We include msggen.pl, since it won't be generated on Windows.
EXTRA_DIST = instmac.pl msggen.pl jade.dsw jade.mak jade-generate.mak

dist-hook:
	if test -f $(top_srcdir)/update-changelog; then \
	  cd $(top_srcdir); \
	  ./update-changelog; \
	  cp $(top_srcdir)/ChangeLog $(top_distdir); \
        fi
	
dist-zip: dist-dir
	-chmod -R a+r $(distdir)
	zip -rql $(distdir).zip $(distdir)
	-rm -rf $(distdir)
