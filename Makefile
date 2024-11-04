.POSIX:

all dep tests install uninstall: check_config FORCE
	+@$(MAKE) -f main.mk $@

check_config: FORCE
	@if ! test -f config.mk;\
	then\
		$(MAKE) config;\
	fi

config: FORCE
	./scripts/config
	$(MAKE) -f main.mk config

config-musl: FORCE
	$(MAKE) LIBPROFILE=musl config
	gcc -v 2>&1 |\
	sed -En '/COLLECT_LTO_WRAPPER=/ s/.*=(.*)\/lto-wrapper/\1/p' |\
	(read -r path; \
	 printf "g/define GCCLIBPATH/ s@.*@#define GCCLIBPATH \"$$path/\"@\nw\n") |\
	ed -s include/bits/scc/sys.h

clean: FORCE
	touch config.mk
	$(MAKE) -f main.mk clean

distclean: FORCE
	touch config.mk
	$(MAKE) -f main.mk distclean
	rm -f config.mk

FORCE:
