#include <wchar.h>

#include "../../libc.h"

int
_mbsset(mbstate_t *ps, int ch)
{
	return ps->__opaque1 = ch;
}
