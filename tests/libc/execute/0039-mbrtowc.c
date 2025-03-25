#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "mbtest.h"

/*
output:
testing
testing mbrtowc1
testing mbrtowc2
testing mbtowc
done
end:
*/

#define NELEM(x) (sizeof(x)/sizeof(x[0]))

void
tests_mbrtowc(void)
{
	struct mbtest *tp;
	int r;
	mbstate_t s;

	puts("testing mbrtowc1");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		wc = -1;
		errno = 0;
		r = mbrtowc(tp->pwc, tp->s, tp->l, NULL);
		assert(tp->r == r);
		assert(tp->syserr == errno);
		if (tp->r != -1)
			assert(tp->wc == wc);
	}

	puts("testing mbrtowc2");
	memset(&s, 0, sizeof(s));
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		wc = -1;
		errno = 0;
		r = mbrtowc(tp->pwc, tp->s, tp->l, &s);
		assert(tp->r == r);
		assert(tp->syserr == errno);
		if (tp->r != -1)
			assert(tp->wc == wc);
		assert(mbsinit(&s) != 0 == tp->mbstate);
	}
}

void
tests_mbtowc(void)
{
	struct mbtest *tp;
	int r, rt;

	puts("testing mbtowc");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		wc = -1;
		errno = 0;
		r = mbtowc(tp->pwc, tp->s, tp->l);
		assert(tp->syserr == errno);
		if (tp->r >= 0) {
			rt = tp->r;
			assert(tp->wc == wc);
		} else {
			rt = -1;
		}
		assert(rt == r);
	}
}

int
main()
{
	puts("testing");
	tests_mbrtowc();
	tests_mbtowc();
	puts("done");
	return 0;
}
