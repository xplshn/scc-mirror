#include <stdio.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

Map *
coff32loadmap(Obj *obj, FILE *fp)
{
	int nsec;
	unsigned long o, s;
	unsigned long long b, e;

	Map *map;
	FILE *src;
	SCNHDR *scn;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	nsec = hdr->f_nscns;
	if ((map = newmap(nsec, 0)) == NULL)
		return NULL;

	for (scn = coff->scns; nsec--; ++scn) {
		b = scn->s_paddr;
		e = b + scn->s_size;

		if (scn->s_scnptr != 0) {
			s = scn->s_size;
			o = obj->pos + scn->s_scnptr;
			src = fp;
		} else {
			s = o = 0;
			src = NULL;
		}

		if (mapsec(map, scn->s_name, src, b, e, s, o) < 0)
			return NULL;
	}

	return map;
}
