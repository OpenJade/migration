## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = check-news dist-zip

## FIXME groveoa is missing, since an .am with only EXTRA_DIST doesn't work.
SUBDIRS = jadedoc dsssl grove spgrove style jade intl po

pkginclude_HEADERS = config.h

BUILT_SOURCES = config.h

## Note that we include the windows-only aux files.
EXTRA_DIST = instmac.pl msggen.pl jade.dsw jade.mak jade-generate.mak


