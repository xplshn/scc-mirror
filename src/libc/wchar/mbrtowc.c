#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "../libc.h"

#undef mbrtowc

size_t
mbrtowc(wchar_t *restrict pwc, const char *restrict s, size_t n,
        mbstate_t *restrict ps)
{
	static mbstate_t state;
	const unsigned char *t = (const unsigned char *) s;
	wchar_t dummy;
	unsigned long wc;
	unsigned c, oc;
	int sh, max;

	if (!ps)
		ps  = &state;

	if (t == NULL) {
		if (ps->sh != 0)
			goto return_error;
		pwc = &dummy;
		goto return_code_set;
	}
	if (n == 0)
		return -2;

	oc = ps->oc;
	wc = ps->wc;
	sh = ps->sh;

	/* initial state? */
	if (sh == 0) {
		/* NUL character? */
		if ((c = wc = *t) == 0)
			goto return_code;
		t++;
		n--;

		/* fast track for ascii? */
		if (c < 0x80)
			goto return_code;

		/* out of sequence multibyte? */
		if ((c & 0xc0) != 0xc0)
			goto return_error;

		/* in sequence multibyte! */
		oc = c << 1;
		wc = 0;
		sh = 1;
	}

	for ( ; n > 0; --n) {
		if (sh > MB_CUR_MAX)
			goto return_error;

		c = *t++;
		if ((c & 0xc0) != 0x80)
			goto return_error;

		wc <<= 6;
		wc |= c & 0x3f;
		oc <<= 1;
		sh++;

		if ((oc & 0x80) == 0) {
			oc = (oc & 0xff) >> sh;
			wc |= oc << (sh-1) * 6;
	
			if (!_validutf8(wc, &max) || sh != max)
				goto return_error;
			goto return_code_set;
		}
	}

	ps->sh = sh;
	ps->oc = oc;
	ps->wc = wc;
	return -2;

return_code_set:
	memset(ps, 0, sizeof(*ps));
return_code:
	if (pwc)
		*pwc = wc;
	return t - (unsigned char *) s;

return_error:
	memset(ps, 0, sizeof(*ps));
	errno = EILSEQ;
	return -1;
}
