#include <wchar.h>

#undef wmemset

wchar_t *
wmemset(wchar_t *d, wchar_t wc, size_t n)
{
	wchar_t *ret = d;

	while (n-- > 0)
		*d++ = wc;
	return ret;
}
