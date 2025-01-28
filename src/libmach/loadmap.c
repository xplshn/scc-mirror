#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static Map *(*ops[NFORMATS])(Obj *, FILE *) = {
	[COFF32] = coff32loadmap,
	[ELF] = elfloadmap,
};

Map *
loadmap(Obj *obj, FILE *fp)
{
	return (*ops[objfmt(obj)])(obj, fp);
}
