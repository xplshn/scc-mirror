.POSIX:

DIRS =\
	amd64-sysv\
	arm64-sysv\
	i386-sysv\
	qbe\
	qbe_amd64-sysv\
	qbe_arm64-sysv\
	z80-scc\

PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

MORE_LDLIBS = -lscc

QBE_OBJS =\
	code.o\
	main.o\
	node.o\
	parser.o\
	sethi.o\
	swtch.o\
	symbol.o\

OBJS =\
	cfg.o\
	$(QBE_OBJS)\

QBE_AMD64_SYSV_OBJS=\
	qbe_amd64-sysv/builtin.o\
	$(QBE_OBJS)\

QBE_ARM64_SYSV_OBJS=\
	qbe_arm64-sysv/builtin.o\
	$(QBE_OBJS)\

Z80_SCC_OBJS =\
	z80-scc/builtin.o\
	$(OBJS) \

TARGET  =\
	cc2-qbe_amd64-sysv\
	cc2-qbe_arm64-sysv\
	cc2-z80-scc\

all:
	+@$(MAKE) $(DIRS)
	+@$(MAKE) $(TARGET)
	@cp $(TARGET) $(LIBEXEC)/scc

main.o: error.h
qbe_amd64-sysv qbe_arm64-sysv: qbe
qbe_amd64-sysv: amd64-sysv
qbe_arm64-sysv: arm64-sysv

error.h: cc2.h
	rm -f $@;\
	trap 'rm -f $$$$.h' EXIT INT QUIT TERM HUP;\
	awk -f generror.awk cc2.h > $$$$.h && mv $$$$.h $@

cc2-qbe_amd64-sysv: $(LIBSCC) $(QBE_AMD64_SYSV_OBJS)
	$(CC) $(PROJ_LDFLAGS) $(QBE_AMD64_SYSV_OBJS) $(PROJ_LDLIBS) -o $@

cc2-qbe_arm64-sysv: $(LIBSCC) $(QBE_ARM64_SYSV_OBJS)
	$(CC) $(PROJ_LDFLAGS) $(QBE_ARM64_SYSV_OBJS) $(PROJ_LDLIBS) -o $@

cc2-z80-scc: $(LIBSCC) $(Z80_SCC_OBJS)
	$(CC) $(PROJ_LDFLAGS) $(Z80_SCC_OBJS) $(PROJ_LDLIBS) -o $@

clean:
	rm -f error.h
