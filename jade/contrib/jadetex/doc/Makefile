install.pdf: install.sgm
	openjade -t tex -V tex-backend -d /home/rahtz/Home/docbook/print/docbook.dsl install.sgm
	pdfjadetex install.tex
	pdfjadetex install.tex

clean:
	-rm *.aux *.log *.dvi
