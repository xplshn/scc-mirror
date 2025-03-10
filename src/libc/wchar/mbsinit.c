#include <wchar.h>

#include "../libc.h"

#undef mbsinit

int
mbsinit(const mbstate_t *ps)
{
	if (!ps)
		return 1;
	return _mbsget(ps) == 0;
}
