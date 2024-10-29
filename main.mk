.POSIX:

DIRS  =\
	src\
	src/libc\
	src/libcrt\
	include/bits/scc\
	tests\

PROJECTDIR = .
include scripts/rules.mk

ROOT = $(DESTDIR)$(PREFIX)
NODEP = 1

all:
	+@$(MAKE) -f main.mk toolchain
	+@$(MAKE) -f main.mk $(ARCH)

config: FORCE
	+@cd include/bits/scc && $(MAKE)

install: all
	$(SCRIPTDIR)/install $(ROOT)
	+@$(MAKE) -f main.mk install-$(ARCH)

uninstall:
	$(SCRIPTDIR)/uninstall $(ROOT)
	+@$(MAKE) -f main.mk uninstall-$(ARCH)

toolchain: src
libc: src/libc
libcrt: src/libcrt

src src/libc src/libcrt: dirs

dirs: $(SCRIPTDIR)/libc-dirs
	xargs mkdir -p < $(SCRIPTDIR)/libc-dirs
	touch dirs

clean:
	xargs rm -rf < $(SCRIPTDIR)/libc-dirs
	find . -name '*.gcno' -o -name '*.gcda' | xargs rm -f
	rm -rf dirs coverage

distclean: clean
	rm -f include/bits/scc/cstd.h
	rm -f include/bits/scc/sys.h
	rm -f include/bits/scc/config.h

include scripts/amd64.mk
include scripts/arm.mk
include scripts/arm64.mk
include scripts/i386.mk
include scripts/ppc.mk
