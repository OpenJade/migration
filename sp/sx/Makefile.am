## Process this file with automake to produce Makefile.in

bin_PROGRAMS = osx

osx_SOURCES = sx.cxx XmlOutputEventHandler.cxx XmlOutputEventHandler.h \
	sx_inst.cxx XmlOutputMessages.h SxMessages.h

EXTRA_DIST = XmlOutputMessages.msg XmlOutputMessages.rc \
	XmlOutputMessages.po SxMessages.msg SxMessages.rc SxMessages.po \
	sx_inst.m4 sx.rc sx.dsp

osx_LDADD = $(top_builddir)/lib/libosp.la

INCLUDES = -I$(top_srcdir)/include

BUILT_SOURCES = XmlOutputMessages.h XmlOutputMessages.rc \
	XmlOutputMessages.po SxMessages.h SxMessages.rc SxMessages.po \
	sx_inst.cxx

MSGGENFLAGS = -l appModule

sx.o: SxMessages.h
XmlOutputEventHandler.o: XmlOutputMessages.h


SUFFIXES = .msg .m4 .rc .po

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@

%.h %.cxx %.rc %.po: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<
