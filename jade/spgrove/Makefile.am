## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libospgrove.la

libospgrove_la_LDFLAGS = -version-info 1:0:0

libospgrove_la_SOURCES = GroveApp.cxx GroveApp.h GroveBuilder.cxx \
	GroveBuilder.h SdNode.cxx SdNode.h threads.h

pkginclude_HEADERS = GroveApp.h GroveBuilder.h

INCLUDES = -I$(top_srcdir)/grove

EXTRA_DIST = grove_inst.m4 grove_inst.cxx spgrove.dsp

BUILT_SOURCES = grove_inst.cxx

MAINTAINERCLEANFILES = Makefile.in

SUFFIXES = .m4 

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@
