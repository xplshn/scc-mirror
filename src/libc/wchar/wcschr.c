#include <wchar.h>

#undef wcschr

wchar_t *
wcschr(const wchar_t *s, wchar_t c)
{
	while (*s && *s != c)
		++s;
	return (*s == c) ? (wchar_t *) s : NULL;
}
