bin_PROGRAMS = onsgmls

onsgmls_SOURCES = nsgmls.cxx SgmlsEventHandler.cxx SgmlsEventHandler.h \
	RastEventHandler.cxx RastEventHandler.h StringSet.cxx StringSet.h \
	nsgmls_inst.cxx NsgmlsMessages.h RastEventHandlerMessages.h

EXTRA_DIST = nsgmls_inst.m4 NsgmlsMessages.msg RastEventHandlerMessages.msg

onsgmls_LDADD = $(top_builddir)/lib/libosp.a

INCLUDES = $(top_srcdir)/include

BUILT_SOURCES = nsgmls_inst.cxx NsgmlsMessages.h RastEventHandlerMessages.h

nsgmls.o: NsgmlsMessages.h
RastEventHandler.o: RastEventHandlerMessages.h

SUFFIXES = .msg .m4

.m4.cxx:
	$(PERL) $(top_srcdir)/lib/instmac.pl $< >$@

%.h %.cxx %.rc: %.msg
	$(PERL) -w $(top_srcdir)/msggen.pl $(MSGGENFLAGS) $<
