#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

int
mapseg(Map *map, Section *seg, FILE *fp, long fsiz)
{
	int n;
	char *s;
	Mapsec *mseg, *mp;

	mp = NULL;
	n = map->nseg;
	for (mseg = map->seg; n--; mseg++) {
		s = mseg->sec.name;
		if (mseg->used && s && !strcmp(s, seg->name))
			goto found;
		if (!mp && !mseg->used)
			mp = mseg;
	}

	if (mp) {
		mseg = mp;
		goto found;
	}

	if ((map = remap(map, map->nsec+1, map->nseg)) == NULL)
		return -1;
	mseg = &map->sec[map->nsec-1];

found:
	mseg->sec = *seg;
	mseg->used = 1;
	mseg->fp = fp,
	mseg->fsiz = fsiz;

	return 0;
}
