# OpenJade testsuite makefile
# Copyright (C) 1999 Avi Kivity

JADE=../jade/jade
JADEX=$(JADE) -c ../dsssl/catalog

all: expr-lang.expected
	@echo All tests passed.

%.expected: %.actual
	diff $< $@

expr-lang.actual: $(JADE)
	$(JADEX) -t sgml -d expr-lang.dsl null.sgml > $@
    
clean:
	rm *.actual
