#include <wchar.h>

#undef wcspbrk

wchar_t *
wcspbrk(const wchar_t *s1, const wchar_t *s2)
{
	s1 += wcscspn(s1, s2);
	return (*s1 != L'\0') ? (wchar_t *) s1 : NULL;
}
