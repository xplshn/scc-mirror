#include <wchar.h>

#undef wcsncmp

int
wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
	unsigned long l1, l2;

	for ( ; n > 0 && *s1 == *s2; --n)
		++s1, ++s2;

	if (n == 0)
		return 0;
	l1 = *s1, l2 = *s2;

	return l1 - l2;
}
