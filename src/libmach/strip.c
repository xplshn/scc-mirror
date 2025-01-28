#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static int (*ops[NFORMATS])(Obj *) = {
	[COFF32] = coff32strip,
};

int
strip(Obj *obj)
{
	return (*ops[objfmt(obj)])(obj);
}
