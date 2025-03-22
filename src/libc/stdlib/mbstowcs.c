#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#undef mbstowcs

size_t
mbstowcs(wchar_t *restrict dest, const char *restrict src, size_t n)
{
	static mbstate_t st;

	return mbsrtowcs(dest, (void *) &src, n, &st);
}
