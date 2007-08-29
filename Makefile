# Time-stamp: <07/01/23 14:12:04 ptr>
#
# Copyright (c) 2006, 2007
# Petr Ovtchenkov
#
# Licensed under the Academic Free License version 3.0
#

SRCROOT := .
SUBDIRS := lib nsgmls sgmlnorm spam spcat spent sx

include ${SRCROOT}/Makefiles/gmake/subdirs.mak

all dbg-shared install depend clean clobber distclean mostlyclean maintainer-clean check:
	+$(call doinsubdirs,${SUBDIRS})

.PHONY: all install depend clean clobber distclean mostlyclean maintainer-clean check
