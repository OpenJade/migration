bin_PROGRAMS = ospam

ospam_SOURCES = spam.cxx CopyEventHandler.cxx CopyEventHandler.h \
	MarkupEventHandler.cxx MarkupEventHandler.h spam_inst.cxx \
	SpamMessages.h

EXTRA_DIST = spam_inst.m4 SpamMessages.msg

ospam_LDADD = $(top_builddir)/lib/libosp.la

INCLUDES = -I$(top_srcdir)/include

BUILT_SOURCES = spam_inst.cxx SpamMessages.h

spam.o: SpamMessages.h


SUFFIXES = .msg .m4

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<

