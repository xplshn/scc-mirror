#include <limits.h>
#include <string.h>
#include <wchar.h>

#undef mbsrtowcs

size_t
mbsrtowcs(wchar_t *restrict dest, const char **restrict src, size_t len,
          mbstate_t *restrict ps)
{
	wchar_t wc;
	const char *s = *src;
	size_t cnt, n;
	static mbstate_t st;

	if (!ps)
		ps = &st;

	n = 0;
	for (;;) {
		cnt = mbrtowc(&wc, s, MB_LEN_MAX, ps);
		if (cnt == (size_t) -2) {
			s += MB_LEN_MAX;
			continue;
		}
		if (cnt == (size_t) -1)
			return -1;

		if (dest) {
			if (n == len) {
				*src = s;
				break;
			}
			*dest++ = wc;
		}

		if (wc == 0) {
			if (dest)
				*src = NULL;
			break;
		}

		s += cnt;
		n++;

	}

	return n;
}
