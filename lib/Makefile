MAKEARCH = make
DIRS = libnvram
%:
	for dir in $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir $@; done
