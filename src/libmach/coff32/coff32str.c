#include <stdio.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#define NAMELEN 8

union name {
	char name[NAMELEN];  /* name */
	struct {
		long zeroes;  /* if name[0-3] == 0 */
		long offset;  /* offset into string table */
	} s;
};

char *
coff32str(Coff32 *coff, void *hdr)
{
	union name *p = hdr;

	if (p->s.zeroes != 0)
		return p->name;
	return &coff->strtbl[p->s.offset];
}
