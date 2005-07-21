MAX_TEX_RECURSION=4
JADE=openjade
JADETEX=jadetex
PDFJADETEX=pdfjadetex
XMLDECL=/usr/share/sgml/xml.dcl
DB2MANXSL=/usr/share/sgml/docbook/xsl-stylesheets/manpages/docbook.xsl


docdir = $(datadir)/doc
pkgdocdir = $(docdir)/@PACKAGE@

man_MANS = onsgmls.1 osgmlnorm.1 ospent.1 ospam.1 osx.1 ospcat.1
BUILT_SOURCES = $(man_MANS)
pkgdoc_DATA = releasenotes.html releasenotes.pdf releasenotes.ps

%.tex: %.xml %.dsl
	$(JADE) -v -t tex -V tex-backend -d $*.dsl#print $(XMLDECL) $*.xml

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
	pdf2ps $< $@

%.html: %.xml %.dsl
	$(JADE) -v -t sgml -i html -V nochunks -V rootchunk -V "%root-filename%=$*" -V "%html-ext%=.html" -d $*.dsl#html $(XMLDECL) $*.xml

%.1: %.xml
	xmlto man -x $(DB2MANXSL) $*.xml
   
mostlyclean-local:
	-rm -f *.out *.aux *.log *.dvi *.tex

clean-local: mostlyclean
	-rm -f *.1 *.html *.pdf *.ps

distclean-local: clean
	-rm -f Makefile Makefile.in
