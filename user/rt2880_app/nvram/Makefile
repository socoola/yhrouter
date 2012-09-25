NVDIR = $(ROOTDIR)/lib/libnvram
CFLAGS += -I$(NVDIR) 
LDFLAGS += -lnvram
EXEC = nvram_daemon ralink_init reset_daemon 3GInfo 


all: $(EXEC)

ifdef CONFIG_RT2880_L2_MANAGE
nvram_daemon: md5c.o rmmsg.o ramad.o nvram_daemon.o
else
nvram_daemon: nvram_daemon.o
endif
	$(CC) $(CFLAGS)  -o $@ $^ $(LDLIBS) $(LDFLAGS)

ralink_init: ralink_init.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)
reset_daemon: reset_daemon.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
#power_led_daemon: power_led_daemon.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
3GInfo: 3GInfo.o serial.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o $(EXEC)

romfs:
	for i in $(EXEC) ; do $(ROMFSINST) /bin/$$i ; done
	ln -sf ralink_init nvram_get
	ln -sf ralink_init nvram_set
	ln -sf ralink_init nvram_bufget
	ln -sf ralink_init nvram_bufset
#	ln -sf ralink_init debug_nvram_get
#	ln -sf ralink_init debug_nvram_set
	mv -f nvram_get $(ROMFSDIR)/bin
	mv -f nvram_set $(ROMFSDIR)/bin
	mv -f nvram_bufget $(ROMFSDIR)/bin
	mv -f nvram_bufset $(ROMFSDIR)/bin
#	mv -f debug_nvram_get $(ROMFSDIR)/bin
#	mv -f debug_nvram_set $(ROMFSDIR)/bin

