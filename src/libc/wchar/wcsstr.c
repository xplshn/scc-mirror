#include <wchar.h>

#undef wcsstr

wchar_t *
wcsstr(const wchar_t *s1, const wchar_t *s2)
{
	wchar_t *p = (wchar_t *) s1;
	wchar_t c = *s2;
	size_t len;

	if ((len = wcslen(s2)) == 0)
		return p;

	for ( ; p = wcschr(p, c); ++p) {
		if (!wcsncmp(p, s2, len))
			return p;
	}

	return NULL;
}

