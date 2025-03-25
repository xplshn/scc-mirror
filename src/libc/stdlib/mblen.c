#include <stdlib.h>
#include <wchar.h>

#undef mblen

int
mblen(const char *s, size_t n)
{
	int ret;
	static mbstate_t st;

	ret = mbrtowc(NULL, s, n, &st);
	if (ret < 0)
		ret = -1;
	return ret;
}
