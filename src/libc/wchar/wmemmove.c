#include <string.h>
#include <wchar.h>

#undef wmemmove

wchar_t *
wmemmove(wchar_t *d, const wchar_t *s, size_t n)
{
	return memmove(d, s,  n * sizeof(wchar_t));
}
