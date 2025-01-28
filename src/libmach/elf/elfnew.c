#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

int
elfnew(Obj *obj, int type)
{
	struct elf *elf;

	if ((elf = calloc(1, sizeof(*elf))) == NULL)
		return -1;
	obj->data = elf;
	obj->index = "/";
	return 0;
}
