all: test-tables ex-table span fence aftermath

ex-table:
	openjade -t tex -d /home/rahtz/Home/docbook/print/docbook.dsl ex-table.sgm
	pdfjadetex ex-table.tex

test-tables:
	openjade -t tex -d /home/rahtz/Home/docbook/print/docbook.dsl test-tables.sgm
	pdfjadetex test-tables.tex

span:
	openjade -t tex -d span.dsl span.sgm
	pdfjadetex span.tex

fence:
	openjade -t tex -d fence.dsl fence.sgm
	pdfjadetex fence.tex

aftermath:
	openjade -t tex -d aftermath.dsl aftermath.sgm
	pdfjadetex aftermath.tex

clean:
	-rm *.ps *.dvi *.log *.aux
	-rm *.pdf
