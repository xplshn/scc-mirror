#include <wchar.h>

#undef wcsxfrm

size_t
wcsxfrm(wchar_t *dst, const wchar_t *src, size_t n)
{
	size_t len = wcslen(src);

	if (len < n)
		wcscpy(dst, src);
	return len;
}
