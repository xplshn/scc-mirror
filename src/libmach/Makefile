.POSIX:

DIRS =\
	elf\
	coff32\

PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = libmach.a

OBJS =\
	archive.o\
	armember.o\
	copysec.o\
	delmap.o\
	delobj.o\
	findsec.o\
	getindex.o\
	getsec.o\
	getsym.o\
	loadmap.o\
	newmap.o\
	newobj.o\
	objpos.o\
	objprobe.o\
	objtype.o\
	pack.o\
	pc2line.o\
	readobj.o\
	rebase.o\
	setindex.o\
	mapsec.o\
	mapseg.o\
	setsec.o\
	setsym.o\
	strip.o\
	unpack.o\
	writeobj.o\

all:
	+@$(MAKE) $(DIRS)
	+@$(MAKE) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(PROJ_ARFLAGS) $@ $?
	$(RL) $(PROJ_RLFLAGS) $@
	cp $@ $(LIBDIR)/scc

dep:
	./mkdep elf coff32
