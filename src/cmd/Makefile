.POSIX:

DIRS   =\
	scc-ld\
	scc-as\
	scc-cc\
	scc-make\
	scc-objdump\

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET =\
	scc-nm\
	scc-ar\
	scc-strip\
	scc-size\
	scc-ranlib\
	scc-dump\
	scc-objcopy\
	scc-addr2line\

MORE_LDLIBS = -lmach -lscc

all: $(TARGET) $(DIRS)
	@cp $(TARGET) $(BINDIR)

$(TARGET): $(LIBMACH) $(LIBSCC)
