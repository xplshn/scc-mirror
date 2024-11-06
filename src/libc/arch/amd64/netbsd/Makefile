.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS  =\
	_brk.$O\
	_close.$O\
	_exit.$O\
	_fork.$O\
	_getpid.$O\
	_gettimeofday.$O\
	_kill.$O\
	_lseek.$O\
	_open.$O\
	_read.$O\
	_setcontext.$O\
	_sigaction2.$O\
	_write.$O\
	_wait4.$O\

OBJS =\
	$(GENOBJS)\
	_sys_errlist.$O\

all: $(OBJS) $(CRT)

crt.$O: ../crt-posix.s ../netbsd/crt.s

$(GENOBJS): syscall.lst
	./gensys.sh $(@:.$O=)
	$(AS) $(PROJ_ASFLAGS) -o $@ $(@:.$O=.s)

clean:
	rm -f $(GENOBJS:.$O=.s) _sys_errlist.c
