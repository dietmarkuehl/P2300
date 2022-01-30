COMPILER = gcc

BUILDDIR = build/$(COMPILER)

.PHONY: default distclean clean config build test

default: config build test

config: 
	-mkdir -p $(BUILDDIR)
	cd $(BUILDDIR); cmake ../..

build: config
	cd $(BUILDDIR); $(MAKE)

test: build
	cd $(BUILDDIR); ./p2300_test

clean:
	$(RM) mkerr olderr

distclean: clean
	cd $(BUILDDIR); $(MAKE) clean
