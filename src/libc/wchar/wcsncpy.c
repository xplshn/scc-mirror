#include <wchar.h>

#undef wcsncpy

wchar_t *
wcsncpy(wchar_t *restrict s1, const wchar_t *restrict s2, size_t n)
{
	wchar_t *ret = s1;

	for (; n > 0 && *s2; --n)
		*s1++ = *s2++;

	while (n-- > 0)
		*s1++ = '\0';

	return ret;

}
