bin_PROGRAMS = osx

osx_SOURCES = sx.cxx XmlOutputEventHandler.cxx XmlOutputEventHandler.h \
	sx_inst.cxx XmlOutputMessages.h SxMessages.h

EXTRA_DIST = XmlOutputMessages.msg SxMessages.msg sx_inst.m4

osx_LDADD = $(top_builddir)/lib/libosp.a

INCLUDES = $(top_srcdir)/include

BUILT_SOURCES = XmlOutputMessages.h SxMessages.h sx_inst.cxx

sx.o: SxMessages.h
XmlOutputEventHandler.o: XmlOutputMessages.h


SUFFIXES = .msg .m4

.m4.cxx:
	$(PERL) $(top_srcdir)/lib/instmac.pl $< >$@

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<
