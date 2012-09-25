DIRS = RT3052
%:
	for dir in $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir $@; done
