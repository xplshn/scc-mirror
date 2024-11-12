.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET =\
	scc-cc\
	scc-cpp\
	scc\

all: $(TARGET)

scc-cc: $(LIBSCC) cc.o
	$(CC) $(PROJ_LDFLAGS) cc.o -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)

scc-cpp: scc-cpp.sh
	cp scc-cpp.sh $@
	chmod +x $@
	cp $@ $(BINDIR)

scc: scc.sh
	cp scc.sh $@
	chmod +x $@
	cp $@ $(BINDIR)
