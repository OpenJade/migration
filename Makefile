# Generated automatically from Makefile.in by configure.
#
#  Makefile.in - Makefile template for Jade
# 
#  $Id$
#
#  Copyright (C) 1998 Cees A. de Groot
#

# Original SP Makefile.in Copyright (C) 1996 Henry S. Thompson
# Original SP Makefile Copyright (c) 1994, 1995, 1996 James Clark

TOP=/free/CVS/OpenJade/jade
prefix=/usr/local.peano
exec_prefix=${prefix}
srcdir=.
bindir=${exec_prefix}/bin
datadir=${prefix}/share

LIBTOOL=$(TOP)/libtool

SP_LIBDIRS=lib $(XLIBDIRS)
LIBDIRS=lib grove spgrove style
PROGDIRS=spcat jade nsgmls spam sgmlnorm spent sx 
dodirs=$(LIBDIRS) $(PROGDIRS)

# Automatic template instantiation can cause compilers to generate
# various extra files; the clean target won't delete these.
TARGETS=all install depend gen clean pure
do=all

$(TARGETS): FORCE
	@$(MAKE) -f Makefile do=$@ $(dodirs)
	@if test $@ = 'install'; then \
	  $(LIBTOOL) --finish $(libdir); \
	  mkdir -p $(datadir); \
	  cp -f dsssl/builtins.dsl $(datadir); \
	fi;

$(LIBDIRS): FORCE
	@if test $(srcdir) = .; \
	then srcdir=.; \
	else srcdir=`cd $(srcdir); pwd`/$@; \
	fi; \
	test -d $@ || mkdir $@; \
	cd $@; \
	test -f $$srcdir/Makefile.dep || touch $$srcdir/Makefile.dep; \
	test -f $$srcdir/Makefile.lt || \
	    $(MAKE) srcdir=$$srcdir VPATH=$$srcdir \
		-f $$srcdir/../Makefile.comm -f $$srcdir/Makefile.sub \
	        -f $$srcdir/../Makefile.lib -f $$srcdir/Makefile.dep \
		Makefile.lt; \
	$(MAKE) srcdir=$$srcdir VPATH=$$srcdir \
		-f $$srcdir/../Makefile.comm -f $$srcdir/Makefile.sub \
		-f $$srcdir/Makefile.lt -f $$srcdir/../Makefile.lib \
		-f $$srcdir/Makefile.dep $(do)

$(PROGDIRS): FORCE
	@if test $(srcdir) = .; \
	then srcdir=.; \
	else srcdir=`cd $(srcdir); pwd`/$@; \
	fi; \
	test -d $@ || mkdir $@; \
	cd $@; \
	test -f $$srcdir/Makefile.dep || touch $$srcdir/Makefile.dep; \
	test -f $$srcdir/Makefile.lt || \
	    $(MAKE) srcdir=$$srcdir VPATH=$$srcdir \
		-f $$srcdir/../Makefile.comm -f $$srcdir/Makefile.sub \
	        -f $$srcdir/../Makefile.prog -f $$srcdir/Makefile.dep \
		Makefile.lt; \
	$(MAKE) srcdir=$$srcdir VPATH=$$srcdir \
		-f $$srcdir/../Makefile.comm -f $$srcdir/Makefile.sub \
		-f $$srcdir/Makefile.lt -f $$srcdir/../Makefile.prog \
		-f $$srcdir/Makefile.dep $(do)

$(PROGDIRS): $(SP_LIBDIRS)

jade: grove spgrove style lib

spgrove: grove lib

style: grove spgrove lib

# GNU tar
TAR=tar

dist: FORCE
	cd test; ./CLEAN
	version=`cat VERSION`; \
	rm -fr sp-$$version; \
	mkdir sp-$$version; \
	cd sp-$$version; \
	ln -s ../* .; \
	rm sp-$$version; \
	rm SP.mak ; \
	sed -e '/^   CD /s/[A-Z]:\\.*\\//' -e "s/$$/`echo @ | tr @ \\\\015`/" \
	  ../SP.mak >SP.mak; \
	cd ..; \
	ln -s `pwd` sp-$$version; \
	$(TAR) -c -f sp-$$version.tar.gz -h -z \
	  --exclude sp-$$version/test/cap \
	  --exclude sp-$$version/test/out \
	  --exclude sp-$$version/test/err \
	  `sed -e "s|.*|sp-$$version/&|" FILES`; \
	rm -fr sp-$$version

mrproper:
	rm -f Makefile Makefile.comm Makefile.lib Makefile.prog \
		config.cache config.log config.status libtool
	find . -name Makefile.dep | xargs rm -f
	test ! -f Makefile.dist || mv Makefile.dist Makefile
	test ! -f Makefile.comm.dist || mv Makefile.comm.dist Makefile.comm
	test ! -f Makefile.prog.dist || mv Makefile.prog.dist Makefile.prog
	test ! -f Makefile.lib.dist || mv Makefile.lib.dist Makefile.lib

FORCE: