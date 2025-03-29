.POSIX:

ROOT=../../../
BINDIR=$(ROOT)/bin

all:
	PATH=$(BINDIR):$$PATH ./chktest.sh

tests:
	@$(MAKE) |\
	grep -v TODO |\
	grep FAIL >/dev/null &&\
	printf '[FAIL]' || printf '[PASS]'; printf '\tas/execute\n'

clean:
	rm -f test.log
