UNAME := $(shell uname)

ifeq ($(UNAME), Minix)
	MAKE=gmake
else
	MAKE=make
endif


all: shell test doc

.PHONY: shell test doc clean

shell:
	$(MAKE) -C src shell

test: shell
	$(MAKE) -C src test

clean:
	$(MAKE) -C src clean
	rm -rf doc/report.log doc/report.aux

doc:
	cd doc && pdflatex report.tex
