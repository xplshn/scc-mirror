#include <errno.h>
#include <stdlib.h>
#include <wchar.h>

#include "../libc.h"

#undef mbrtowc

size_t
mbrtowc(wchar_t *restrict pwc, const char *restrict s, size_t n,
        mbstate_t *restrict ps)
{
	const unsigned char *t = (const unsigned char *) s;
	unsigned long wc;
	unsigned c;
	int i, len, maxlen;

	if (t == NULL)
		return 0;
	if ((wc = *t) == 0)
		goto return_code;

	c = *t++;
	for (len = 0; n > 0 && c & 0x80; --n, ++len)
		c <<= 1;
	if (n == 0 && c & 0x80)
		return -2;
	if (len == 1 || len > MB_CUR_MAX)
		goto return_error;
	if (len == 0)
		goto return_code;

	wc = (c & 0xFF) >> len;
	for (i = 0; i < len-1; i++) {
		if (((c = *t++) & 0xC0) != 0x80)
			goto return_error;
		wc <<= 6;
		wc |= c & 0x3F;
	}

	if (!_validutf8(wc, &maxlen) || len != maxlen)
		goto return_error;

return_code:
	if (pwc)
		*pwc = wc;
	return t - (unsigned char *) s;

return_error:
	errno = EILSEQ;
	return -1;
}
