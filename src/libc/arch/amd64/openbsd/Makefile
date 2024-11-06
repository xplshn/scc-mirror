.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_access.$O\
	_brk.$O\
	_close.$O\
	_execve.$O\
	_exit.$O\
	_fork.$O\
	_getpid.$O\
	_getrusage.$O\
	_gettimeofday.$O\
	_kill.$O\
	_lseek.$O\
	_open.$O\
	_read.$O\
	_rename.$O\
	_sigaction.$O\
	_unlink.$O\
	_write.$O\
	_wait4.$O\

OBJS =\
	$(GENOBJS)\
	_sys_errlist.$O\

all: $(OBJS) $(CRT)

$(CRT): ../crt-posix.s ../openbsd/crt.s

$(GENOBJS): syscall.lst
	./gensys.sh $(@:.$O=)
	$(AS) $(PROJ_ASFLAGS) -o $@ $(@:.$O=.s)

clean:
	rm -f $(GENOBJS:.$O=.s) _sys_errlist.c
