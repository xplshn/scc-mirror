#include <wchar.h>

#undef mbsinit

int
mbsinit(const mbstate_t *ps)
{
	if (!ps)
		return 1;
	return ps->oc == 0;
}
