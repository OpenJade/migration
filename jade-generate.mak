# This is a Makefile for nmake that makes all the .cxx and .h files that
# are automatically generated.  It's too painful to do in the IDE.
# You'll need m4 and perl to use this.

M4=m4
PERL=perl

GENSRCS=\
style/style_inst.cxx \
style/FlowObj_inst.cxx \
style/primitive_inst.cxx \
style/common_inst.cxx \
spgrove/grove_inst.cxx \
jade/HtmlFOTBuilder_inst.cxx \
jade/RtfFOTBuilder_inst.cxx \
jade/TransformFOTBuilder_inst.cxx \
jade/JadeMessages.h \
jade/HtmlMessages.h \
jade/RtfMessages.h \
jade/TeXMessages.h \
style/DssslAppMessages.h \
style/InterpreterMessages.h

.SUFFIXES: .m4 .msg

all: $(GENSRCS)


.m4.cxx:
	rm -f $@
	$(M4) lib\instmac.m4 $< >$@
	chmod -w $@

.msg.h:
	rm -f $@
	$(PERL) -w msggen.pl $<
	chmod -w $@
