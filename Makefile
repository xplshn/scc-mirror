.POSIX:

all tests install uninstall: check_config FORCE
	+@$(MAKE) -f main.mk $@

check_config: FORCE
	@if ! test -f config.mk;\
	then\
		$(MAKE) config;\
	fi

config: FORCE
	./scripts/config
	$(MAKE) -f main.mk config

clean: FORCE
	touch config.mk
	$(MAKE) -f main.mk clean

distclean: FORCE
	touch config.mk
	$(MAKE) -f main.mk distclean
	rm -f config.mk

FORCE:
