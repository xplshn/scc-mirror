#include <wchar.h>

#include "../../libc.h"

int
_mbsget(mbstate_t *ps)
{
	return ps->__mbstate8[0];
}
