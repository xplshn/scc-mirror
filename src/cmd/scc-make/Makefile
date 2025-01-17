.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	defaults.o\
	main.o\
	parser.o\
	rules.o\
	$(DRIVER).o\

TARGET = scc-make

all: $(TARGET)

scc-make: $(OBJS)
	$(CC) $(PROJ_LDFLAGS) $(OBJS) $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)
