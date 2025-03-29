.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

NODEP = 1

all: config.h sys.h cstd.h

cstd.h: FORCE
	cp cstd-$(STD).h $@

config.h: FORCE
	rm -f $@;\
	trap "rm -f $$$$.h" EXIT INT QUIT HUP TERM;\
	(echo '#define PREFIX "$(PREFIX)"';\
	 echo '#define ARCH "$(ARCH)"';\
	 echo '#define SYS  "$(SYS)"';\
	 echo '#define ABI  "$(ABI)"';\
	 echo '#define LIBPREFIX "$(LIBPREFIX)"';\
	 echo '#define FORMAT "$(FORMAT)"') > $$$$.h && mv $$$$.h $@

sys.h: FORCE
	rm -f $@;\
	trap "rm -f $$$$.tmp" INT QUIT TERM HUP;\
	sed -e 's/%NOPIE%/"$(NOPIE_LDFLAGS)",/' \
	    -e 's/"",//' sys-$(LIBPROFILE).h > $$$$.tmp && \
	mv $$$$.tmp $@
