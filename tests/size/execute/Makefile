.POSIX:

BINDIR = ../../../bin/

AS = z80-unknown-coff-as
OUT = z80.out

all: $(OUT)
	PATH=$(BINDIR):$$PATH ./chktest.sh

tests:
	@if command -v $(AS) >/dev/null;\
	then\
		$(MAKE) | \
		grep -v TODO |\
		grep FAIL >/dev/null &&\
		printf '[FAIL]' || printf '[PASS]';\
	else\
		printf '[SKIP]';\
	fi;\
	printf '\tsize/execute\n'

z80.out: master.s
	$(AS) -o $@ master.s

clean:
	rm -f *.a *.out test.log
