#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

Map *
remap(Map *map, int nsec, int nseg)
{
	struct mapsec *sec, *seg;

	if (nseg > SIZE_MAX/sizeof(*sec) || nseg > SIZE_MAX/sizeof(*seg))
		return NULL;

	if (nseg == 0) {
		free(map->seg);
		seg = NULL;
	} else {
		seg = realloc(map->seg, nseg * sizeof(*seg));
		if (!seg)
			return NULL;
	}
	map->seg = seg;
	map->nseg = nseg;

	if (nsec == 0) {
		free(map->seg);
		sec = NULL;
	} else {
		sec = realloc(map->seg, nsec * sizeof(*sec));
		if (!sec)
			return NULL;
	}
	map->sec = sec;
	map->nsec = nsec;

	return map;
}

Map *
newmap(int nsec, int nseg)
{
	Map m, *map;
	struct mapsec *sec, *seg;

	if (!remap(memset(&m, 0, sizeof(Map)), nsec, nseg))
		return NULL;

	if ((map = malloc(sizeof(*map))) == NULL) {
		free(m.sec);
		free(m.seg);
		return NULL;
	}

	*map = m;
	return map;
}
