#include <stdio.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

Map *
coff32loadmap(Obj *obj, FILE *fp)
{
	int i, nsec;
	Section sec;
	Map *map;
	FILE *src;
	SCNHDR *scn;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	nsec = hdr->f_nscns;
	if ((map = newmap(nsec, 0)) == NULL)
		return NULL;

	for (i = 0; getsec(obj, &i, &sec); ++i) {
		sec.offset += obj->pos;
		src = ((sec.flags & SALLOC) != 0) ? fp : NULL;

		if (mapsec(map, &sec, src, sec.size) < 0)
			return NULL;
	}

	return map;
}
