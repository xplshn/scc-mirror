.POSIX:

all: deps

deps:
	$(MAKE) dep;\
	touch $@;\

all dep tests install uninstall: check_config FORCE
	+@$(MAKE) -f main.mk $@

check_config: FORCE
	@if ! test -f config.mk;\
	then\
		$(MAKE) config;\
	fi

config: FORCE
	@if ! command -v qbe > /dev/null;\
	then \
		echo "qbe code generator is required to use scc" >&2;\
		exit 1;\
	fi
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

distclean: clean FORCE
	touch config.mk
	$(MAKE) -f main.mk distclean
	rm -f config.mk deps

FORCE:
