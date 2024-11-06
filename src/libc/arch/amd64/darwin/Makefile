.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	__close.$O\
	__exit.$O\
	__fork.$O\
	__getpid.$O\
	__kill.$O\
	__lseek.$O\
	__open.$O\
	__read.$O\
	__sigaction.$O\
	__write.$O\

OBJS  =\
	$(GENOBJS)\
	_sys_errlist.$O\

all: $(OBJS) $(CRT)

$(CRT): ../crt-posix.s

$(GENOBJS): syscall.lst
	./gensys.sh $(@:.$O=)
	$(AS) $(PROJ_ASFLAGS) -o $@ $(@:.$O=.s)

clean:
	rm -f $(GENOBJS:.$O=.s) _sys_errlist.c
