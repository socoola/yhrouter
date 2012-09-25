MAKEARCH = make
DIRS = libnvram

romfs:
	$(ROMFSINST) /lib
%:
	for dir in $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir $@; done
