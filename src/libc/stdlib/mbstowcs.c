#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#undef mbstowcs

/*
 * we can reuse the internal state of mbstwocws() because
 * we loop in case of having a -2 from mbrtowc(), that
 * makes impossible to leave the function in a non initial
 * state (or error that cleans the state).
 */
size_t
mbstowcs(wchar_t *restrict dest, const char *restrict src, size_t n)
{
	return mbsrtowcs(dest, (void *) &src, n, NULL);
}
