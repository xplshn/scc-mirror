#include <wchar.h>

#undef mbrlen

size_t
mbrlen(const char *restrict s, size_t n, mbstate_t *restrict ps)
{
	static mbstate_t st;

	if (!ps)
		ps = &st;
	return mbrtowc(NULL, s, n, ps);
}
