## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = dist-zip check-news

## FIXME: all is missing since a .am with only EXTRA_DIST doesn't work.
SUBDIRS = pubtext doc \
	include generic \
	lib nsgmls spent spam sgmlnorm sx spcat \
	intl po

pkginclude_HEADERS = config.h

docdir = $(prefix)/doc
pkgdocdir = $(docdir)/@PACKAGE@

pkgdoc_DATA = NEWS COPYING AUTHORS ChangeLog

BUILT_SOURCES = config.h msggen.pl

## Note that we include the windows-only aux files.
## We include msggen.pl, since it won't be generated on Windows.
EXTRA_DIST = instmac.pl msggen.pl sunfix.sh SP.dsw SP.mak \
	build-win32.bat sp-generate.mak

