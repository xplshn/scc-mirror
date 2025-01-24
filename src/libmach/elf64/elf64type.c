#include <stdio.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/elf64.h>

#include "../libmach.h"
#include "fun.h"

int
elf64type(char *name)
{
	struct arch *ap;

	for (ap = elf64archs; ap ->name; ap++) {
		if (strcmp(ap->name, name) == 0)
			return ap->type;
	}

	return -1;
}
