SCRIPTS = $(notdir $(wildcard *.sh))

all:

clean:

romfs:
	sed -e '/^#/d' ${ROOTDIR}/${CONFIG_LINUXDIR}/.config > .config&&sed -e '/^#/d' ${ROOTDIR}/config/.config >> .config    
	chmod 755 .config
	$(ROMFSINST) .config /sbin/config.sh
	for i in $(SCRIPTS) ; do \
		$(ROMFSINST) $$i /sbin/$$i ; \
	done

ifeq ("$(CONFIG_USER_STORAGE)", "")	
	rm $(ROMFSDIR)/sbin/storage.sh
endif

ifeq ("$(CONFIG_USB)", "")	
	rm $(ROMFSDIR)/sbin/automount.sh
endif

ifeq ("$(CONFIG_USER_3G)", "")	
	rm $(ROMFSDIR)/sbin/3g.sh
	rm $(ROMFSDIR)/sbin/3gdialup_led.sh
	rm $(ROMFSDIR)/sbin/config-3g-ppp.sh
	
endif

ifeq ("$(CONFIG_USER_GOAHEAD_GreenAP)", "")	
	rm $(ROMFSDIR)/sbin/greenap.sh
endif

ifeq ("$(CONFIG_USER_INADYN)", "")	
	rm $(ROMFSDIR)/sbin/ddns.sh
endif

ifeq ("$(CONFIG_USER_PPPD)", "")	
	rm $(ROMFSDIR)/sbin/config-pppoe.sh
endif

ifeq ("$(CONFIG_USER_RP_L2TP)", "")	
	rm $(ROMFSDIR)/sbin/config-l2tp.sh
endif

ifeq ("$(CONFIG_USER_PPTP_CLIENT)", "")	
	rm $(ROMFSDIR)/sbin/config-pptp.sh
endif
