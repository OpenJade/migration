#
#  Makefile.in - Makefile template for Jade
# 
#  $Id$
#
#  Copyright (C) 1998 Cees A. de Groot
#

# Original SP Makefile.in Copyright (C) 1996 Henry S. Thompson
# Original SP Makefile Copyright (c) 1994, 1995, 1996 James Clark

top_builddir=/home/ian/OPENJADE/cvs/jade
TOP=/home/ian/OPENJADE/cvs/jade
prefix=/usr
exec_prefix=${prefix}
srcdir=.
bindir=${exec_prefix}/bin
datadir=/usr/share/sgml/openjade-1.3.1
libdir=${exec_prefix}/lib

DESTDIR=

LIBTOOL=$(SHELL) $(top_builddir)/libtool
LIBTOOL_DEPS=config/ltmain.sh

SP_LIBDIRS=lib $(XLIBDIRS)
LIBDIRS=lib grove spgrove style
PROGDIRS=nsgmls spam sgmlnorm spent jade sx
dodirs=$(LIBDIRS) $(PROGDIRS)

# Automatic template instantiation can cause compilers to generate
# various extra files; the clean target won't delete these.
TARGETS=all install depend gen clean pure
do=all

$(TARGETS): FORCE
	@$(MAKE) -f Makefile do=$@ $(dodirs)
	@if test $@ = 'install'; then \
	  $(LIBTOOL) --finish $(DESTDIR)$(libdir); \
	  mkdir -p $(DESTDIR)$(datadir); \
	  cp -f dsssl/builtins.dsl $(DESTDIR)$(datadir); \
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
	-rm -f Makefile Makefile.comm Makefile.lib Makefile.prog \
		config.cache config.log config.status
	-test -f Makefile.dist && mv Makefile.dist Makefile
	-test -f Makefile.comm.dist && mv Makefile.comm.dist Makefile.comm
	-test -f Makefile.prog.dist && mv Makefile.prog.dist Makefile.prog
	-test -f Makefile.lib.dist && mv Makefile.lib.dist Makefile.lib

FORCE:

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

