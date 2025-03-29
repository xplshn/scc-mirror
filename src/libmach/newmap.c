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
	int n;
	struct mapsec *sec, *seg;

	if (nseg > SIZE_MAX/sizeof(*sec) || nseg > SIZE_MAX/sizeof(*seg))
		return NULL;

	n = nseg - map->nseg;
	if (nseg == 0) {
		free(map->seg);
		seg = NULL;
	} else if (n == 0) {
		seg = map->seg;
	} else {
		seg = realloc(map->seg, nseg * sizeof(*seg));
		if (!seg)
			return NULL;
		if (n > 0) {
			memset(&seg[map->nseg], 0, n * sizeof(*seg));
		}
	}
	map->seg = seg;
	map->nseg = nseg;

	n = nsec - map->nsec;
	if (nsec == 0) {
		free(map->seg);
		sec = NULL;
	} else if (n == 0) {
		sec = map->sec;
	} else {
		sec = realloc(map->sec, nsec * sizeof(*sec));
		if (!sec)
			return NULL;
		if (n > 0)
			memset(&sec[map->nsec], 0, n * sizeof(*sec));
	}
	map->sec = sec;
	map->nsec = nsec;

	return map;
}

Map *
newmap(int nsec, int nseg)
{
	Map m, *map;

	if (!remap(memset(&m, 0, sizeof(Map)), nsec, nseg))
		goto err;

	if ((map = malloc(sizeof(*map))) == NULL)
		goto err;

	*map = m;
	return map;

err:
	free(m.sec);
	free(m.seg);
	return NULL;
}
