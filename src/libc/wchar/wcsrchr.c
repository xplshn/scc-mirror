#include <wchar.h>

#undef wcsrchr

wchar_t *
wcsrchr(const wchar_t *s, wchar_t c)
{
	wchar_t *t;

	for (t = (wchar_t *) s; *t; ++t)
		;
	while (t > s && *t != c)
		--t;

	return (*t == c) ? t : NULL;
}
