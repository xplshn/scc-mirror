.POSIX:

BINDIR = ../../../bin/

all:
	PATH=$(BINDIR):$$PATH ./chktest.sh

tests:
	@$(MAKE) |\
	grep -v TODO |\
	grep FAIL >/dev/null &&\
	printf '[FAIL]' || printf '[PASS]'; printf '\tmake/error\n'

hello:
	@echo Hello World!

clean:
	rm -f test.log tmp?.* file? file.c file.o core*
