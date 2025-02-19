#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

void
delmap(Map *map)
{
	free(map->seg);
	free(map->sec);
	free(map);
}
