#include <wchar.h>

#undef wcsspn

size_t
wcsspn(const wchar_t *s, const wchar_t *accept)
{
	wchar_t wc;
        const wchar_t *p;

        for (p = s; (wc = *p) && wcschr(accept, wc); p++)
		;
        return p - s;
}
