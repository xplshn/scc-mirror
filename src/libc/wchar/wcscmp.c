#include <wchar.h>

#undef wcscmp

int
wcscmp(const wchar_t *s1, const wchar_t *s2)
{
	unsigned long l1, l2;

	while (*s1 && *s1 == *s2)
		++s1, ++s2;
	l1 = *s1, l2 = *s2;

	return l1 - l2;
}
