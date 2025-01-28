#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

#include "elf/fun.h"
#include "coff32/fun.h"

static int (*ops[NFORMATS])(unsigned char *, char **) = {
	[COFF32] = coff32probe,
	[ELF] = elfprobe,
};

int
objprobe(FILE *fp, char **name)
{
	int n, t;
	fpos_t pos;
	int (**bp)(unsigned char *, char **);
	unsigned char buf[NBYTES];

	fgetpos(fp, &pos);
	n = fread(buf, NBYTES, 1, fp);
	fsetpos(fp, &pos);

	if (n != 1 || ferror(fp))
		return -1;

	for (bp = ops; bp < &ops[NFORMATS]; ++bp) {
		if ((t = (**bp)(buf, name)) >= 0)
			return t;
	}

	return -1;
}
