MAX_TEX_RECURSION=4
XMLTO=@XMLTO@
JADE=@JADE@
JADETEX=@JADETEX@
PDF2PS=@PDF2PS@
PDFJADETEX=@PDFJADETEX@
XMLDCL=@XMLDCL@


docdir = $(datadir)/doc
pkgdocdir = $(docdir)/@PACKAGE@

man_MANS = onsgmls.1 osgmlnorm.1 ospent.1 ospam.1 osx.1 ospcat.1
BUILT_SOURCES = $(man_MANS)
pkgdoc_DATA = releasenotes.html releasenotes.pdf releasenotes.ps

%.tex: %.xml %.dsl
	$(JADE) -v -t tex -V tex-backend -d $*.dsl#print $(XMLDCL) $*.xml

%.pdf: %.tex
	$(PDFJADETEX) $<
	if ! cmp $(shell basename $< .tex).aux prior.aux 2>/dev/null && \
		expr $(MAKELEVEL) '<' $(MAX_TEX_RECURSION); then \
		cp -pf $(shell basename $< .tex).aux prior.aux ; \
		rm -f $@ ; \
		$(MAKE) $@ ; \
	fi
	rm -f prior.aux

%.dvi: %.tex
	$(JADETEX) $<
	if ! cmp $(shell basename $< .tex).aux prior.aux 2>/dev/null && \
		expr $(MAKELEVEL) '<' $(MAX_TEX_RECURSION); then \
		cp -pf $(shell basename $< .tex).aux prior.aux ; \
		rm -f $@ ; \
		$(MAKE) $@ ; \
	fi
	rm -f prior.aux

%.ps: %.pdf
	$(PDF2PS) $< $@

%.html: %.xml %.dsl
	$(JADE) -v -t sgml -i html -V nochunks -V rootchunk -V "%root-filename%=$*" -V "%html-ext%=.html" -d $*.dsl#html $(XMLDCL) $*.xml

%.1: %.xml
	$(XMLTO) man $*.xml
   
mostlyclean-local:
	-rm -f *.out *.aux *.log *.dvi *.tex

clean-local: mostlyclean
	-rm -f *.1 *.html *.pdf *.ps

distclean-local: clean
	-rm -f Makefile Makefile.in
