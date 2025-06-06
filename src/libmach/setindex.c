#include <errno.h>
#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static int (*ops[NFORMATS])(long, char **, long *, FILE *) = {
	[COFF32] = coff32setidx,
};

int
setindex(int type, long nsyms, char **names, long *offs, FILE *fp)
{
	int fmt;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS) {
		errno = ERANGE;
		return -1;
	}

	return (*ops[fmt])(nsyms, names, offs, fp);
}

