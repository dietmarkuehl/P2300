COMPILER = gcc

BUILDDIR = build/$(COMPILER)

.PHONY: default clean config build test

default: config build test

config: 
	-mkdir -p $(BUILDDIR)
	cd $(BUILDDIR); cmake ../..

build: config
	cd $(BUILDDIR); $(MAKE)

test: build
	cd $(BUILDDIR); ./p2300_test

clean:
	cd $(BUILDDIR); $(MAKE) clean
	$(RM) mkerr olderr
