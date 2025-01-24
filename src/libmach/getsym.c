#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf64/fun.h"
#include "coff32/fun.h"

static Symbol *(*ops[NFORMATS])(Obj *, int *, Symbol *) = {
	[COFF32] = coff32getsym,
	[ELF64] = elf64getsym,
};

Symbol *
getsym(Obj *obj, int *index, Symbol *sym)
{
	return (*ops[objfmt(obj)])(obj, index, sym);
}
