MAKEFILES:=$(wildcard ./*/Makefile)

figures: $(MAKEFILES)
	$(MAKE) -C feynman
	$(MAKE) -C tikz

# clean unnecessary files
clean:
	$(MAKE) -C feynman clean
	$(MAKE) -C tikz clean

# remove all products
cleaner: clean
	$(MAKE) -C feynman cleaner
	$(MAKE) -C tikz cleaner
