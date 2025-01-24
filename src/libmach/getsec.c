#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf64/fun.h"
#include "coff32/fun.h"

static Section *(*ops[NFORMATS])(Obj *, int *, Section *) = {
	[COFF32] = coff32getsec,
	[ELF64] = elf64getsec,
};

Section *
getsec(Obj *obj, int *idx, Section *sec)
{
	return (*ops[objfmt(obj)])(obj, idx, sec);
}
