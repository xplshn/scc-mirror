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

OBJS =\
	main.o\
	parser.o\
	peep.o\
	symbol.o\
	node.o\
	code.o\
	optm.o\

TARGET  =\
	cc2-amd64-sysv\
	cc2-i386-sysv\
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

cc2-amd64-sysv: $(LIBSCC) $(OBJS) amd64-sysv/builtin.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) amd64-sysv/builtin.o $(PROJ_LDLIBS) -o $@

cc2-i386-sysv: $(LIBSCC) $(OBJS) i386-sysv/builtin.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) i386-sysv/builtin.o $(PROJ_LDLIBS) -o $@

cc2-qbe_amd64-sysv: $(LIBSCC) $(OBJS) qbe_amd64-sysv/builtin.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) qbe_amd64-sysv/builtin.o $(PROJ_LDLIBS) -o $@

cc2-qbe_arm64-sysv: $(LIBSCC) $(OBJS) qbe_arm64-sysv/builtin.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) qbe_arm64-sysv/builtin.o $(PROJ_LDLIBS) -o $@

cc2-z80-scc: $(LIBSCC) $(OBJS) z80-scc/builtin.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) z80-scc/builtin.o $(PROJ_LDLIBS) -o $@

clean:
	rm -f error.h
