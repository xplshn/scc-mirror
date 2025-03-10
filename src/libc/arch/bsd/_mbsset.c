#include <wchar.h>

#include "../../libc.h"

int
_mbsset(mbstate_t *ps, int ch)
{
	return ps->__mbstate8[0] = ch;
}
