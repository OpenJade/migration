## Process this file with automake to produce Makefile.in

bin_PROGRAMS = openjade

openjade_SOURCES = jade.cxx SgmlFOTBuilder.cxx RtfFOTBuilder.cxx \
	HtmlFOTBuilder.cxx TeXFOTBuilder.cxx TransformFOTBuilder.cxx \
	MifFOTBuilder.cxx SgmlFOTBuilder.h RtfFOTBuilder.h \
	HtmlFOTBuilder.h TeXFOTBuilder.h TransformFOTBuilder.h \
	MifFOTBuilder.h TextFOTBuilder.h TextFOTBuilder.cxx \
	JadeMessages.h HtmlMessages.h RtfMessages.h TeXMessages.h \
	HtmlFOTBuilder_inst.cxx RtfFOTBuilder_inst.cxx TeXFOTBuilder_inst.cxx \
	TransformFOTBuilder_inst.cxx MifMessages.h MifFOTBuilder_inst.cxx \
	TmpOutputByteStream.h

openjade_LDADD = $(top_builddir)/style/libostyle.la \
	$(top_builddir)/spgrove/libospgrove.la \
	$(top_builddir)/grove/libogrove.la -losp

INCLUDES = -I$(top_srcdir)/grove -I$(top_srcdir)/spgrove -I$(top_srcdir)/style

EXTRA_DIST = JadeMessages.msg JadeMessages.rc JadeMessages.po \
	HtmlMessages.msg HtmlMessages.rc HtmlMessages.po \
	RtfMessages.msg RtfMessages.rc RtfMessages.po \
	TeXMessages.msg TeXMessages.rc TeXMessages.po \
	MifMessages.msg MifMessages.rc MifMessages.po \
	HtmlFOTBuilder_inst.m4 RtfFOTBuilder_inst.m4 \
	TeXFOTBuilder_inst.m4 TransformFOTBuilder_inst.m4 \
	MifFOTBuilder_inst.m4 jade.dsp jade.rc \
	RtfOle.cxx RtfOle.h

BUILT_SOURCES = JadeMessages.h JadeMessages.rc JadeMessages.po \
	HtmlMessages.h HtmlMessages.rc HtmlMessages.po \
	RtfMessages.h RtfMessages.rc RtfMessages.po \
	TeXMessages.h TeXMessages.rc TeXMessages.po \
	MifMessages.h MifMessages.rc MifMessages.po \
	HtmlFOTBuilder_inst.cxx RtfFOTBuilder_inst.cxx \
	TeXFOTBuilder_inst.cxx TransformFOTBuilder_inst.cxx \
	MifFOTBuilder_inst.cxx

MSGGENFLAGS = -l xModule

SUFFIXES = .msg .m4 .rc .po

.m4.cxx:
	$(PERL) $(top_srcdir)/instmac.pl $< >$@

%.h %.cxx %.rc %.po: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<
