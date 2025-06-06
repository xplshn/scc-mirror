amd64:
	+@$(MAKE) -f main.mk CONF=amd64-linux libc libcrt
	+@$(MAKE) -f main.mk CONF=amd64-openbsd libc libcrt
	+@$(MAKE) -f main.mk CONF=amd64-netbsd libc libcrt
	+@$(MAKE) -f main.mk CONF=amd64-dragonfly libc libcrt
	+@$(MAKE) -f main.mk CONF=amd64-darwin libc libcrt
	+@$(MAKE) -f main.mk CONF=amd64-freebsd libc libcrt

install-amd64: amd64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.amd64 $(ROOT)

uninstall-amd64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.amd64 $(ROOT)
