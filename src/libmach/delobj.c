#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf64/fun.h"
#include "coff32/fun.h"

static void (*ops[NFORMATS])(Obj *) = {
	[COFF32] = coff32del,
	[ELF64] = elf64del,
};

void
delobj(Obj *obj)
{
	(*ops[objfmt(obj)])(obj);
	free(obj);
}
