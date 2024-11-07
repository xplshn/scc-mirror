.POSIX:

PROJECTDIR = ../../../../..
RULES = user

include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_access.$O\
	_close.$O\
	_creat.$O\
	_execve.$O\
	_exit.$O\
	_fork.$O\
	_getpid.$O\
	_kill.$O\
	_link.$O\
	_open.$O\
	_read.$O\
	_sys_brk.$O\
	_unlink.$O\
	_write.$O\
	_wait4.$O\

OBJS =\
	$(GENOBJS)\
	_cerrno.$O\
	_sys_errlist.$O\

all: $(CRT) $(OBJS)

$(CRT): ../crt-posix.s

$(GENOBJS): syscall.lst
	./gensys.sh $(@:.$O=)
	$(AS) $(PROJ_ASFLAGS) -o $@ $(@:.$O=.s)

clean:
	rm -f $(GENOBJS:.$O=.s) _sys_errlist.c
