## Makefile for the tests subdirectory of OpenSP
## Copyright (C) 2002 OpenSP Project
## Process this file with automake to produce Makefile.in.

# AUTOMAKE_OPTIONS = 1.5 no-dependencies

SHOWSTOPPERS = af-7 af-8 af-9

TESTS = onsgmls-1 \
osnorm-1 \
af-1 af-2 af-3 af-4 af-5 af-6 \
$(SHOWSTOPPERS)

EXTRA_DIST = $(TESTS)

TESTS_ENVIRONMENT = top_srcdir=$(top_srcdir) PATH=.:../nsgmls:$$PATH \
		    ONSGMLS=`echo onsgmls|sed '$(transform)'`\
		    OSGMLNORM=`echo osgmlnorm|sed '$(transform)'`\
		    $(SHELL)

# Two auxiliary programs used by the tests.
INCLUDES = -I.. -I$(top_srcdir)/lib -I$(top_srcdir)/intl
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

