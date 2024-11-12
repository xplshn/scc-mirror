.POSIX:

ROOT=../../../
BINDIR=$(ROOT)/bin

all:
	@BINDIR=$(BINDIR) ./chktest.sh

tests:
	@$(MAKE) |\
	grep -v TODO |\
	grep FAIL >/dev/null &&\
	printf '[FAIL]' || printf '[PASS]'; printf '\tas/execute\n'

clean:
	rm -f test.log
