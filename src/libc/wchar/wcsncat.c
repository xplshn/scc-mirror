#include <wchar.h>

#undef wcsncat

wchar_t *
wcsncat(wchar_t *restrict s1, const wchar_t *restrict s2, size_t n)
{
	wchar_t *ret;

	for (ret = s1; *s1; ++s1)
		;
	while (n-- > 0 && *s2)
		*s1++ = *s2++;
	*s1 = '\0';

	return ret;

}
