#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#undef mbtowc

int
mbtowc(wchar_t *restrict pwc, const char *restrict s, size_t n)
{
	int ret;
	static mbstate_t st;

	ret = mbrtowc(pwc, s, n, &st);
	if (ret == -2) {
		memset(&st, 0, sizeof(st));
		ret = -1;
	}

	return ret;
}
