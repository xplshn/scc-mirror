#include <stdio.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

int
elftype(char *name)
{
	struct arch *ap;

	for (ap = elfarchs; ap ->name; ap++) {
		if (strcmp(ap->name, name) == 0)
			return ap->type;
	}

	return -1;
}
