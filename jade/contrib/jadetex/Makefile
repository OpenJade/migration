default: jadetex.fmt pdfjadetex.fmt 

basic: jadetex.ltx
	tex jadetex.ins

jadetex.fmt: basic
	tex -ini -progname=jadetex "&hugelatex" jadetex.ini

pdfjadetex.fmt: basic
	pdftex -ini -progname=pdfjadetex "&pdflatex" pdfjadetex.ini

install: jadetex.fmt pdfjadetex.fmt
	(TT=`kpsewhich -expand-var '$$TEXMFMAIN'`;cp pdfjadetex.fmt $$TT/web2c)
	(TT=`kpsewhich -expand-var '$$TEXMFMAIN'`;cp jadetex.fmt $$TT/web2c)
	-(TT=`kpsewhich -expand-var '$$TEXMFMAIN'`;\
	mkdir -p $$TT/tex/jadetex ; \
	cp dsssl.def jadetex.ltx $$TT/tex/jadetex)

realclean: clean
	-rm jadetex.fmt pdfjadetex.fmt dsssl.def jadetex.ltx

clean:
	-rm  *.log *.aux *.idx *.pdf *.toc *.dvi *.out

test:
	openjade -t tex -d ../docbook/print/docbook.dsl test-tables.sgm

distrib:
	rm jadetex.zip; (cd ..; zip -r jadetex/jadetex.zip jadetex/*1 jadetex/*.dtx jadetex/*.ins jadetex/*.sty jadetex/*.fd \
	jadetex/doc/install.sgm jadetex/doc/install.pdf \
	jadetex/Makefile jadetex/ChangeLog jadetex/*ini)

jadetex.ltx: jadetex.dtx

