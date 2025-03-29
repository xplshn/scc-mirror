#include <stdio.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

int
coff32setidx(long nsymbols, char *names[], long offs[], FILE *fp)
{
	return coff32xsetidx(BIG_ENDIAN, nsymbols, names, offs, fp);
}
