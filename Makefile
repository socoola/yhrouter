LINUXDIR = linux-2.6.21.x
LIBCDIR  = $(CONFIG_LIBCDIR)
ROOTDIR  = $(shell pwd)
PATH	 := $(PATH):$(ROOTDIR)/tools
HOSTCC   = cc
IMAGEDIR = $(ROOTDIR)/images
ROMFSDIR = $(ROOTDIR)/romfs
MAKEARCH = make
ROMFSINST = romfs-inst.sh
STRIPTOOL = strip


export LINUXDIR ROOTDIR ROMFSDIR MAKEARCH ROMFSINST STRIPTOOL
DIRS = lib user vendors
all:
	for dir in $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir $@; done

romfs: all
	mkdir -p $(ROMFSDIR)
	mkdir -p $(ROMFSDIR)/bin/
	mkdir -p $(ROMFSDIR)/lib/
	mkdir -p $(ROMFSDIR)/etc/
	mkdir -p $(ROMFSDIR)/etc/nvram/
	mkdir -p $(ROMFSDIR)/etc_ro/Wireless/RT2860AP/
	for dir in $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir $@; done

install: romfs
	cp -Rf $(ROMFSDIR)/* /

uninstall:
	echo "not implement"

%:
	for dir in $(DIRS); do [ ! -d $$dir ] || $(MAKEARCH) -C $$dir $@; done

