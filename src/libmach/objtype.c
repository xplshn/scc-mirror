#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static int (*ops[NFORMATS])(char *) = {
	[COFF32] = coff32type,
	[ELF] = elftype,
};

int
objtype(char *name)
{
	int t;
	int (**bp)(char *);

	for (bp = ops; bp < &ops[NFORMATS]; ++bp) {
		if ((t = (**bp)(name)) >= 0)
			return t;
	}

	return -1;
}
