#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

static int
matchseg(Map *map, Mapsec *msec, FILE *fp)
{
	unsigned long long o, e, mo, me;
	Section *seg;
	Mapsec *mseg, **v;

	mo = msec->sec.load;
	me =  mo + msec->sec.size;

	if ((msec->sec.flags & SLOAD) == 0)
		return 0;

	for (mseg = map->seg; mseg < &map->seg[map->nseg]; ++mseg) {
		if (mseg->fp != fp)
			continue;
		o = mseg->sec.load;
		e = o + mseg->sec.size;
		if (mo >= o && me <= e) {
			v = realloc(mseg->child, sizeof(Mapsec *) * (mseg->nchild+1));
			if (!v)
				return -1;
			mseg->child = v;
			v[mseg->nchild++] = msec;
		}
	}

	return 0;
}

int
mapsec(Map *map, Section *sec, FILE *fp, long fsiz)
{
	int n;
	char *s;
	Mapsec *msec, *mp;

	mp = NULL;
	n = map->nsec;
	for (msec = map->sec; n--; msec++) {
		s = msec->sec.name;
		if (s && !strcmp(s, sec->name))
			goto found;
		if (!mp && !msec->used)
			mp = msec;
	}

	if (mp) {
		msec = mp;
		goto found;
	}

	if ((map = remap(map, map->nsec+1, map->nseg)) == NULL)
		return -1;
	msec = &map->sec[map->nsec-1];

found:
	msec->used = 1;
	msec->sec = *sec;
	msec->fp = fp,
	msec->fsiz = fsiz;

	return matchseg(map, msec, fp);
}
