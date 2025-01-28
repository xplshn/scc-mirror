#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static int (*ops[NFORMATS])(Obj *, Map *, FILE *) = {
	[COFF32] = coff32write,
};

int
writeobj(Obj *obj, Map *map, FILE *fp)
{
	return (*ops[objfmt(obj)])(obj, map, fp);
}
