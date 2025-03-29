#include <wchar.h>

#undef wcscoll

int
wcscoll(const wchar_t *s1, const wchar_t *s2)
{
	return wcscmp(s1, s2);
}
