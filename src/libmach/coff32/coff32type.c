#include <stdio.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

int
coff32type(char *name)
{
	struct arch *ap;

	for (ap = coff32archs; ap->name; ap++) {
		if (strcmp(ap->name, name) == 0)
			return ap->type;
	}

	return -1;
}
