i386:
	+@$(MAKE) -f main.mk CONF=i386-linux libc libcrt

install-i386: i386
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.i386 $(ROOT)

uninstall-i386:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.i386 $(ROOT)
