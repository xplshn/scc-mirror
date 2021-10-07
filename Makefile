.POSIX:

DIRS  =\
	src\
	src/libc\
	src/libcrt\
	include/scc/scc\
	tests\

PROJECTDIR = .
include $(PROJECTDIR)/scripts/rules.mk

PREFIX= $(HOME)
NODEP = 1

all:
	+@PREFIX=$(PREFIX) $(MAKE) `$(SCRIPTDIR)/config` toolchain
	+@$(MAKE) `$(SCRIPTDIR)/config` `uname -m`

install:
	$(SCRIPTDIR)/install $(PREFIX)
	+@$(MAKE) install-`uname -m`

uninstall:
	$(SCRIPTDIR)/uninstall $(PREFIX)
	+@$(MAKE) uninstall-`uname -m`

#############################################################
# x86_64 rules

x86_64 amd64:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-linux libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-openbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-netbsd libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-dragonfly libc libcrt
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=amd64-darwin libc libcrt

install-x86_64 install-amd64: amd64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.amd64 $(PREFIX)

uninstall-x86_64 uninstall-amd64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.amd64 $(PREFIX)

#############################################################
# i386 rules

i386:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=i386-linux libc libcrt

install-i386: i386
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.i386 $(PREFIX)

uninstall-i386:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.i386 $(PREFIX)

#############################################################
# ppc rules

ppc:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=ppc-linux libc libcrt

install-ppc: ppc
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.ppc $(PREFIX)

uninstall-ppc:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.ppc $(PREFIX)

#############################################################
# arm rules

arm:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm-linux libc libcrt

install-arm: arm
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.arm $(PREFIX)

uninstall-arm:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.arm $(PREFIX)

#############################################################
# arm64 rules

arm64:
	+@$(MAKE) `$(SCRIPTDIR)/config` CONF=arm64-linux libc libcrt

install-arm64: arm64
	$(SCRIPTDIR)/install -p $(SCRIPTDIR)/proto.arm64 $(PREFIX)

uninstall-arm64:
	$(SCRIPTDIR)/uninstall -p $(SCRIPTDIR)/proto.arm64 $(PREFIX)

toolchain: dirs src
libc: dirs src/libc
libcrt: dirs src/libcrt
src: include/scc/scc
tests: all

dirs: $(SCRIPTDIR)/libc-proto
	xargs mkdir -p < $(SCRIPTDIR)/libc-proto
	touch dirs

clean:
	xargs rm -rf < $(SCRIPTDIR)/libc-proto
	rm -f dirs
