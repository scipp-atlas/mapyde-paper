# find all sources
SRC:=$(shell ls *.tex)
COMMON:=$(wildcard ./*.tex ./*.fmf)

# make list of plots
PDF=$(SRC:%.tex=output/%.pdf)
SVG=$(SRC:%.tex=output/%.svg)
PNG=$(SRC:%.tex=output/%.png)
EPS=$(SRC:%.tex=output/%.eps)

TEXINPUTS:=../../$(add prefix:,$(TEXINPUTS))::
export TEXINPUTS

# by default make all plots
plots: $(PDF)

pdf: $(PDF)
svg: $(SVG)
png: $(PNG)
eps: $(EPS)

%: output/%.pdf;

# rule to make the pdf files
output/%.pdf: %.tex $(COMMON)
	mkdir -p output
	pdflatex -jobname=$* $<
	pdfcrop --margins 3 $*.pdf $@
	$(RM) $*.pdf
	$(RM) $*.aux $*.log

# rule to make the png files
output/%.png: output/pdf/%.pdf
	mkdir -p output
	convert -scale 300x200 -density 600 -quality 100 $< $@

# rule to make the eps files
output/%.eps: output/pdf/%.pdf
	mkdir -p output
	pdf2ps $< $@

# rule to make the svg files
output/%.svg: %.tex $(COMMON)
	mkdir -p output
	latex -jobname=$* $<
	test $*TMP && mpost $*TMP
	latex -jobname=$* $<
	dvisvgm --no-fonts --output=%f $*.dvi
	mv $*.svg $@
	$(RM) $*.dvi
	$(RM) $*TMP* $*.aux $*.log

# clean unnecessary files
clean:
	$(RM) *.aux *.log *.toc *.out *.mp *.1 *.t1 *.dvi *.bcf *.run.xml

# remove all products
cleaner: clean
	$(RM) -r output/
	$(RM) *.pdf
