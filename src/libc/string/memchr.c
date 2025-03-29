#include <string.h>

#undef memchr

void *
memchr(const void *s, int c, size_t n)
{
	unsigned char *bp = (unsigned char *) s;

	for ( ; n > 0 && *bp != c; n--)
		++bp;
	return (n == 0) ? NULL : bp;
}
