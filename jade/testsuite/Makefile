# OpenJade testsuite makefile
# Copyright (C) 1999 Avi Kivity

JADE=../jade/openjade
JADEX=$(JADE) -c ../dsssl/catalog

all: expr-lang.expected contains.expected
	@echo All tests passed.

%.expected: %.actual
	diff $< $@

expr-lang.actual: $(JADE)
	$(JADEX) -t sgml -d expr-lang.dsl null.sgml > $@
    
contains.actual: $(JADE)
	$(JADEX) -t fot -d contains.dsl -o $@ contains.sgml 
clean:
	rm *.actual
