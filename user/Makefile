DIRS = goahead rt2880_app
LDFLAGS := -L$(ROOTDIR)/lib/lib 
CFLAGS := -DRUN_ON_PC 

export LDFLAGS CFLAGS
%:
	for dir in $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir $@; done

