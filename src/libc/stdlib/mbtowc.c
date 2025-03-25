#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#undef mbtowc

int
mbtowc(wchar_t *restrict pwc, const char *restrict s, size_t n)
{
	static mbstate_t st;
	int ret;

	ret = mbrtowc(pwc, s, n, &st);
	if (ret < 0)
		ret = -1;

	return ret;
}
