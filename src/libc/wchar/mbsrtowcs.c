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

	for (n = 0; ; n++) {
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
				return n;
			}
			*dest++ = wc;
		}
		s += cnt;

		if (wc == 0)
			break;
	}

	if (dest) {
		memset(ps, 0, sizeof(mbstate_t));
		*src = NULL;
	}

	return n;
}
