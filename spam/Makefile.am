## Process this file with automake to produce Makefile.in

bin_PROGRAMS = ospam

ospam_SOURCES = spam.cxx CopyEventHandler.cxx CopyEventHandler.h \
	MarkupEventHandler.cxx MarkupEventHandler.h spam_inst.cxx \
	SpamMessages.h

EXTRA_DIST = SpamMessages.msg SpamMessages.rc \
	spam.rc spam.dsp spam_inst.m4 

ospam_LDADD = $(top_builddir)/lib/libosp.la

INCLUDES = -I$(top_srcdir)/include

BUILT_SOURCES = spam_inst.cxx SpamMessages.h SpamMessages.rc 

MSGGENFLAGS = -l appModule

spam.o: SpamMessages.h


SUFFIXES = .msg .m4 .rc

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_builddir)/msggen.pl $(MSGGENFLAGS) $<

