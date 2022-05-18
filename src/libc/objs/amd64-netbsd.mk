include objs/common-objs.mk

OBJS =\
	$(COMMON_OBJS)\
	arch/amd64/longjmp.$O\
	arch/amd64/memchr.$O\
	arch/amd64/memcmp.$O\
	arch/amd64/memcpy.$O\
	arch/amd64/memmove.$O\
	arch/amd64/memset.$O\
	arch/amd64/netbsd/_brk.$O\
	arch/amd64/netbsd/_close.$O\
	arch/amd64/netbsd/_exit.$O\
	arch/amd64/netbsd/_fork.$O\
	arch/amd64/netbsd/_getpid.$O\
	arch/amd64/netbsd/_kill.$O\
	arch/amd64/netbsd/_lseek.$O\
	arch/amd64/netbsd/_open.$O\
	arch/amd64/netbsd/_read.$O\
	arch/amd64/netbsd/_setcontext.$O\
	arch/amd64/netbsd/_sigaction2.$O\
	arch/amd64/netbsd/_sys_errlist.$O\
	arch/amd64/netbsd/_wait4.$O\
	arch/amd64/netbsd/_write.$O\
	arch/amd64/netbsd/crt.$O\
	arch/amd64/setjmp.$O\
	arch/amd64/strchr.$O\
	arch/amd64/strcmp.$O\
	arch/amd64/strcpy.$O\
	arch/bsd/_waitpid.$O\
	arch/netbsd/_sigaction.$O\
	arch/posix/_getheap.$O\
	arch/posix/_open.$O\
	arch/posix/_systime.$O\
	arch/posix/_tzone.$O\
	arch/posix/clock.$O\
	arch/posix/getenv.$O\
	arch/posix/raise.$O\
	arch/posix/signal.$O\
	arch/posix/system.$O\
	arch/posix/time.$O\
	string/strlen.$O\
