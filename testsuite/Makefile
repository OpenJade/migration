# OpenJade testsuite makefile
# Copyright (C) 1999 Avi Kivity

JADE=../jade/openjade
JADEX=$(JADE) -c ../dsssl/catalog

all: expr-lang.expected contains.expected specificity.expected query-root.expected
	@echo All tests passed.

%.expected: %.actual
	diff $< $@

expr-lang.actual: $(JADE)
	$(JADEX) -t sgml -d expr-lang.dsl null.sgml > $@
    
contains.actual: $(JADE)
	$(JADEX) -t fot -d contains.dsl -o $@ contains.sgml 

specificity.actual: $(JADE)
	$(JADEX) -t fot -d specificity.dsl -o $@ specificity.sgml 

query-root.actual: $(JADE)
	$(JADEX) -t fot -d query-root.dsl -o $@ query-root.sgml 

clean:
	rm *.actual
