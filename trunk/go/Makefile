ARCH=6

%: %.go
	$(ARCH)g $*.go && $(ARCH)l -o $* $*.$(ARCH)
	rm *.[58]
