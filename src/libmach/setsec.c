#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static Section *(*ops[NFORMATS])(Obj *, int *, Section *) = {
	[COFF32] = coff32setsec,
};

Section *
setsec(Obj *obj, int *idx, Section *sec)
{
	return (*ops[objfmt(obj)])(obj, idx, sec);
}
