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
	struct arch *ap;

	if ((elf = calloc(1, sizeof(*elf))) == NULL)
		return -1;
	for (ap = elfarchs; ap->name; ++ap) {
		if (ap->type == type)
			break;
	}

	elf->is32 = (ap->name) ? ap->is32 : -1;
	obj->data = elf;
	obj->index = "/";
	return 0;
}
