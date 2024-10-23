.POSIX:

DIRS   =\
	ld\
	as\
	cc\
	make\

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET =\
	scc-nm\
	scc-ar\
	scc-strip\
	scc-size\
	scc-ranlib\
	scc-objdump\
	scc-objcopy\
	scc-addr2line\

MORE_LDLIBS = -lmach -lscc

all: $(TARGET) $(DIRS)
	@cp $(TARGET) $(BINDIR)

$(TARGET): $(LIBMACH) $(LIBSCC)

include deps.mk
