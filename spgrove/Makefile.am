## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libospgrove.la

libospgrove_la_LDFLAGS = --version-info 0:0:0
#libospgrove_la_LDFLAGS = --version-info 0:0:0 -lm \
#	-L$(top_builddir)/grove -logrove -losp

libospgrove_la_SOURCES = GroveApp.cxx GroveApp.h GroveBuilder.cxx \
	GroveBuilder.h SdNode.cxx SdNode.h grove_inst.cxx threads.h

INCLUDES = -I$(top_srcdir)/grove

EXTRA_DIST = grove_inst.m4 spgrove.dsp

BUILT_SOURCES = grove_inst.cxx

SUFFIXES = .m4 .msg

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<

