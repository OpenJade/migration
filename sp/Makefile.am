## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = check-news dist-zip

## FIXME: all is missing since a .am with only EXTRA_DIST doesn't work.
SUBDIRS = pubtext doc \
          include generic \
          lib nsgmls spent spam sgmlnorm sx spcat 

## Note that we include the windows-only aux files.
EXTRA_DIST = instmac.pl msggen.pl sunfix.sh SP.dsw SP.mak \
	build-win32.bat sp-generate.mak
