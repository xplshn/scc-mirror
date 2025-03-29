#include <wchar.h>

#undef wcscspn

size_t
wcscspn(const wchar_t *s, const wchar_t *reject)
{
	wchar_t wc;
	const wchar_t *p;

        for (p = s; (wc = *p) && !wcschr(reject, wc); p++)
		;
        return p - s;
}
