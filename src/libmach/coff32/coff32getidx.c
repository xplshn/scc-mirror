#include <stdio.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

int
coff32getidx(long *nsyms, char ***namep, long **offsp, FILE *fp)
{
	return coff32xgetidx(BIG_ENDIAN, nsyms, namep, offsp, fp);
}
