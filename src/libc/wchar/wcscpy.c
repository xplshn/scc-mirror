#include <wchar.h>

#undef wcscpy

wchar_t *
wcscpy(wchar_t * restrict s1, const wchar_t * restrict s2)
{
	wchar_t *ret = s1;

	while ((*s1++ = *s2++) != '\0')
		;

	return ret;

}

