bin_PROGRAMS = ospcat

ospcat_SOURCES = spcat.cxx spcat_inst.cxx

EXTRA_DIST = spcat_inst.m4

ospcat_LDADD = $(top_builddir)/lib/libosp.la

INCLUDES = -I$(top_srcdir)/include

BUILT_SOURCES = spcat_inst.cxx

SUFFIXES = .msg .m4

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<

