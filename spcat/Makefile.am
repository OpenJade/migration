## Process this file with automake to produce Makefile.in

bin_PROGRAMS = ospcat

ospcat_SOURCES = spcat.cxx spcat_inst.cxx SpcatMessages.h

EXTRA_DIST = spcat_inst.m4 spcat.dsp SpcatMessages.msg SpcatMessages.rc

ospcat_LDADD = $(top_builddir)/lib/libosp.la

INCLUDES = -I$(top_srcdir)/include

BUILT_SOURCES = spcat_inst.cxx SpcatMessages.h SpcatMessages.rc

MSGGENFLAGS = -l appModule

SUFFIXES = .m4 .msg .rc

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<
