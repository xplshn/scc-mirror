#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/*
output:
testing
testing wcrtomb1
testing wcrtomb2
testing wctomb
done
end:
*/

#define NELEM(x) (sizeof(x)/sizeof(x[0]))

static char str[MB_CUR_MAX+1];

static struct wctest {
	wchar_t wc;
	char *s;
	char exp[MB_CUR_MAX+1];
	int r;
	int syserr;
	int mbstate;
} tests[] = {
	{0,        NULL, "",                 1, 0,      1},
	{0,         str, "\0",               1, 0,      1},
	{0x21,      str, "\x21",             1, 0,      1},
	{0x00A1,    str, "\xc2\xa1",         2, 0,      1},
	{0x2014,    str, "\xe2\x80\x94",     3, 0,      1},
	{0x01F4A9,  str, "\xf0\x9f\x92\xa9", 4, 0,      1},

	{0xd800,    str, "",                -1, EILSEQ, 1},
	{0xDCFF,    str, "",                -1, EILSEQ, 1},
	{0xDD00,    str, "\xed\xb4\x80",     3, 0,      1},
	{0x10ffff,  str, "\xf4\x8f\xbf\xbf", 4, 0,      1},
	{0x110000,  str, "",                -1, EILSEQ, 1},
};

void
tests_wcrtomb(void)
{
	struct wctest *tp;
	int r;
	mbstate_t s;

	puts("testing wcrtomb1");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		memset(str, 0, MB_CUR_MAX+1);
		errno = 0;
		r = wcrtomb(tp->s, tp->wc, NULL);
		assert(tp->r == r);
		assert(tp->syserr == errno);
		if (tp->s && tp->r != -1)
			assert(!memcmp(tp->s, tp->exp, MB_CUR_MAX+1));
	}

	puts("testing wcrtomb2");
	memset(&s, 0, sizeof(s));
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		memset(str, 0, MB_CUR_MAX+1);
		errno = 0;
		r = wcrtomb(tp->s, tp->wc, &s);
		assert(tp->r == r);
		assert(tp->syserr == errno);
		if (tp->s && tp->r != -1)
			assert(!memcmp(tp->s, tp->exp, MB_CUR_MAX+1));
		assert(mbsinit(&s) != 0 == tp->mbstate);
	}
}

void
tests_wctomb(void)
{
	struct wctest *tp;
	int r;

	puts("testing wctomb");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		memset(str, 0, MB_CUR_MAX+1);
		errno = 0;
		r = wctomb(tp->s, tp->wc);
		assert(tp->r == r);
		assert(tp->syserr == errno);
		if (tp->s && tp->r != -1)
			assert(!memcmp(tp->s, tp->exp, MB_CUR_MAX+1));
	}
}

int
main(void)
{
	puts("testing");
	tests_wcrtomb();
	tests_wctomb();
	puts("done");
	return 0;
}
