## Process this file with automake to produce Makefile.in

bin_PROGRAMS = ospent

ospent_SOURCES = spent.cxx SpentMessages.h

ospent_LDADD = $(top_builddir)/lib/libosp.la

INCLUDES = -I$(top_srcdir)/include

EXTRA_DIST = spent.dsp SpentMessages.msg SpentMessages.rc

BUILT_SOURCES = SpentMessages.msg SpentMessages.rc

MSGGENFLAGS = -l appModule

SUFFIXES = .msg .rc

%.h %.cxx %.rc: %.msg
	[ ! -f $(top_srcdir)/msggen.pl ] || $(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<
