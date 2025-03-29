.POSIX:

PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	coff32archs.o\
	coff32del.o\
	coff32getidx.o\
	coff32getsec.o\
	coff32getsym.o\
	coff32loadmap.o\
	coff32new.o\
	coff32pc2line.o\
	coff32probe.o\
	coff32read.o\
	coff32setidx.o\
	coff32setsec.o\
	coff32setsym.o\
	coff32str.o\
	coff32strip.o\
	coff32type.o\
	coff32write.o\
	coff32xgetidx.o\
	coff32xsetidx.o\

COFFHDRS =\
	$(INCDIR)/bits/scc/coff32/aouthdr.h\
	$(INCDIR)/bits/scc/coff32/filehdr.h\
	$(INCDIR)/bits/scc/coff32/linenum.h\
	$(INCDIR)/bits/scc/coff32/reloc.h\
	$(INCDIR)/bits/scc/coff32/scnhdr.h\
	$(INCDIR)/bits/scc/coff32/storclass.h\
	$(INCDIR)/bits/scc/coff32/syms.h

all: $(OBJS)

$(OBJS): $(COFFHDRS)

dep:
	@{ printf 'COFF32OBJS=\\\n';\
	printf '\tcoff32/%s\\\n' $(OBJS); } > deps.mk

distclean:
	rm -f deps.mk
