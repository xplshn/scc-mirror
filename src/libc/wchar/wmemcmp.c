#include <wchar.h>

#undef wmemcmp

int
wmemcmp(const wchar_t *s, const wchar_t *t, size_t n)
{
	unsigned long ls, lt;

	for (; n > 0 && *s == *t; --n)
		++s, ++t;
	if (n == 0)
		return 0;

	ls = *s, lt = *t;
	return ls - lt;
}
