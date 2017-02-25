all: shell test doc

.PHONY: shell test doc clean

shell:
	cd src && make shell

test: shell
	cd src && make test

clean:
	cd src && make clean
	cd doc && rm -rf report.log report.aux

doc:
	cd doc && pdflatex report.tex
