AS = z80-unknown-coff-as

BINDIR = ../../../bin/

OBJS   = f1.o f2.o
OBJLIB = f3.o

.SUFFIXES: .s

all:
	PATH=$(BINDIR):$$PATH $(MAKE) a.out

.s.o:
	$(AS) -o $@ $<

tests:
	@if command -v $(AS) >/dev/null;\
	then\
		$(MAKE) |\
		grep -v TODO |\
		grep FAIL >/dev/null &&\
		printf '[FAIL]' || printf '[PASS]';\
	else\
		printf '[SKIP]';\
	fi;\
	printf '\tld/execute\n'

a.out: $(OBJS) f.a
	scc ld $(OBJS) f.a >/dev/null 2>&1 || true

f.a: $(OBJLIB)
	scc ar -rv $@ $? >/dev/null 2>&1

size:
	scc size *.o *.a

clean:
	rm -f *.o *.a a.out core* *.dat
