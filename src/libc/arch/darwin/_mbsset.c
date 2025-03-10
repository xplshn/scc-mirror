#include <wchar.h>

#include "../../libc.h"

int
_mbsset(mbstate_t *ps, int ch)
{
	ps-count = 1;
	return ps->state[0] = ch;
}
