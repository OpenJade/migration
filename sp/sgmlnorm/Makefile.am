## Process this file with automake to produce Makefile.in

bin_PROGRAMS = osgmlnorm

osgmlnorm_SOURCES = sgmlnorm.cxx SGMLGenerator.cxx SGMLGenerator.h \
	SgmlnormMessages.h

EXTRA_DIST = sgmlnorm.dsp SgmlnormMessages.msg SgmlnormMessages.rc

BUILT_SOURCES = SgmlnormMessages.h SgmlnormMessages.rc

osgmlnorm_LDADD = $(top_builddir)/lib/libosp.la

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/generic

MSGGENFLAGS = -l appModule

SUFFIXES = .msg .rc

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_builddir)/msggen.pl $(MSGGENFLAGS) $<
