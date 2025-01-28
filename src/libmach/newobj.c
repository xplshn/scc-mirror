#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static int (*ops[NFORMATS])(Obj *, int) = {
	[COFF32] = coff32new,
	[ELF] = elfnew,
};

Obj *
newobj(int type)
{
	Obj *obj;
	int fmt;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS) {
		errno = ERANGE;
		return NULL;
	}

	if ((obj = malloc(sizeof(*obj))) == NULL)
		return NULL;

	obj->type = type;
	obj->next = NULL;
	if ((*ops[fmt])(obj, type) < 0) {
		free(obj);
		return NULL;
	}

	return obj;
}
