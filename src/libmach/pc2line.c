#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static int (*ops[NFORMATS])(Obj *, unsigned long long , char *, int *) = {
	[COFF32] = coff32pc2line,
};

int
pc2line(Obj *obj, unsigned long long pc, char *fname, int *ln)
{
	return (*ops[objfmt(obj)])(obj, pc, fname, ln);
}
