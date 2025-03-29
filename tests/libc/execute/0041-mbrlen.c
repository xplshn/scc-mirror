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
testing mbrlen1
testing mbrlen2
testing mblen
done
end:
*/

#define NELEM(x) (sizeof(x)/sizeof(x[0]))

void
tests_mbrlen(void)
{
	struct mbtest *tp;
	int r;
	mbstate_t s;

	puts("testing mbrlen1");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		wc = -1;
		errno = 0;
		r = mbrlen(tp->s, tp->l, NULL);
		assert(tp->rmbr == r);
		assert(tp->syserr == errno);
	}

	puts("testing mbrlen2");
	memset(&s, 0, sizeof(s));
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		wc = -1;
		errno = 0;
		r = mbrlen(tp->s, tp->l, &s);
		assert(tp->rmbr == r);
		assert(tp->syserr == errno);
		assert(mbsinit(&s) != 0 == tp->mbstate);
	}
}

void
tests_mblen(void)
{
	struct mbtest *tp;
	int r, rt;

	puts("testing mblen");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		r = mblen(tp->s, tp->l);
		assert(tp->rmb == r);
	}
}

int
main()
{
	puts("testing");
	tests_mbrlen();
	tests_mblen();
	puts("done");
	return 0;
}
