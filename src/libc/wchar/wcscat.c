#include <wchar.h>

#undef wcscat

wchar_t *
wcscat(wchar_t *restrict s1, const wchar_t *restrict s2)
{
	wchar_t *p;

	for (p = s1; *p; ++p)
		;
	while ((*p++ = *s2++) != 0)
		;

	return s1;
}
