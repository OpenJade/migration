bin_PROGRAMS = openjade

openjade_SOURCES = jade.cxx SgmlFOTBuilder.cxx RtfFOTBuilder.cxx \
	HtmlFOTBuilder.cxx TeXFOTBuilder.cxx TransformFOTBuilder.cxx \
	MifFOTBuilder.cxx SgmlFOTBuilder.h RtfFOTBuilder.h \
	HtmlFOTBuilder.h TeXFOTBuilder.h TransformFOTBuilder.h \
	MifFOTBuilder.h \
	JadeMessages.h HtmlMessages.h RtfMessages.h TeXMessages.h \
	HtmlFOTBuilder_inst.cxx RtfFOTBuilder_inst.cxx TeXFOTBuilder_inst.cxx \
	TransformFOTBuilder_inst.cxx MifMessages.h MifFOTBuilder_inst.cxx

openjade_LDADD = $(top_builddir)/style/libostyle.la \
	$(top_builddir)/spgrove/libospgrove.la

INCLUDES = -I$(top_srcdir)/grove -I$(top_srcdir)/spgrove -I$(top_srcdir)/style

BUILT_SOURCES = JadeMessages.h HtmlMessages.h RtfMessages.h TeXMessages.h \
	HtmlFOTBuilder_inst.cxx RtfFOTBuilder_inst.cxx TeXFOTBuilder_inst.cxx \
	TransformFOTBuilder_inst.cxx MifMessages.h MifFOTBuilder_inst.cxx

HtmlFOTBuilder.o: HtmlFOTBuilder_inst.cxx HtmlMessages.h
RtfFOTBuilder.o: RtfFOTBuilder_inst.cxx RtfMessages.h
TeXFOTBuilder.o: TeXFOTBuilder_inst.cxx TeXMessages.h
MifFOTBuilder.o: MifFOTBuilder_inst.cxx MifMessages.h
TransformFOTBuilder.o: TransformFOTBuilder_inst.cxx
jade.o: JadeMessages.h

SUFFIXES = .msg .m4

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<
