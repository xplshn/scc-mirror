.POSIX:

DIRS =\
	elf64\
	coff32\

PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = libmach.a

all: $(TARGET)

OBJS =\

all: $(TARGET)
	+@$(MAKE) $(DIRS)
	+@$(MAKE) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(PROJ_ARFLAGS) $@ $?
	$(RL) $(PROJ_RLFLAGS) $@
	cp $@ $(LIBDIR)/scc

dep:
	(echo H;\
	 echo /OBJS/a;\
	 ls *.c */*.c | sed -E 's/(.*)\.c$$/	\1.o\\/';\
	 echo .; \
	 echo w) |\
	ed -s makefile
