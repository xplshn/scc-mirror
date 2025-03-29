#include <string.h>
#include <wchar.h>

#undef wmemcpy

wchar_t *
wmemcpy(wchar_t *restrict d, const wchar_t *restrict s, size_t n)
{
	return memcpy(d, s, n * sizeof(wchar_t));
}
