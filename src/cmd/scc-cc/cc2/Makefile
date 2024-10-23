.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

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

all: $(TARGET)

main.o: error.h

error.h: cc2.h
	rm -f $@;\
	trap 'rm -f $$$$.h' EXIT INT QUIT TERM HUP;\
	awk -f generror.awk cc2.h > $$$$.h && mv $$$$.h $@

clean:
	rm -f target/*/*.o error.h

include amd64-sysv/target.mk
include i386-sysv/target.mk
include qbe_amd64-sysv/target.mk
include qbe_arm64-sysv/target.mk
include z80-scc/target.mk
include deps.mk
