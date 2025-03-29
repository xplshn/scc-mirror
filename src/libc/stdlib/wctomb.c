#include <stdlib.h>
#include <wchar.h>

#undef wctomb

int
wctomb(char *s, wchar_t wc)
{
	static mbstate_t st;

	return wcrtomb(s, wc, &st);
}
