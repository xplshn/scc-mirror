.POSIX:
PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = scc-ld

OBJS =\
	main.o\
	symbol.o\
	section.o\
	pass1.o\
	pass2.o \
#	pass3.o \
#       pass4.o \
#       pass5.o \

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) $(OBJS) -lmach -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)
