#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

int
findsec(Map *map, char *name)
{
	int i;
	Mapsec *sec;

	for (i = 0; i < map->nsec; i++) {
		char *s = map->sec[i].sec.name;
		if (s && strcmp(s, name) == 0)
			return i;
	}

	return -1;
}
