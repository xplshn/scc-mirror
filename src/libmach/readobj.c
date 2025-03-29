#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static int (*ops[NFORMATS])(Obj *, FILE *) = {
	[COFF32] = coff32read,
	[ELF] = elfread,
};

int
readobj(Obj *obj, FILE *fp)
{
	long off;

	if ((off = ftell(fp)) == EOF)
		return -1;
	obj->pos = off;

	return (*ops[objfmt(obj)])(obj, fp);
}
