#include <wchar.h>

wchar_t *
wmemchr(const wchar_t *s, wchar_t c, size_t n)
{
	wchar_t *bp = (wchar_t *) s;

	for ( ; n > 0 && *bp != c; n--)
		++bp;
	return (n == 0) ? NULL : bp;
}
