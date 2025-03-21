#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/*
output:
testing
testing mbsrtowcs1
testing mbsrtowcs2
testing mbstowcs
done
end:
*/

#define NELEM(x) (sizeof(x)/sizeof(x[0]))

static char str[20];
static wchar_t wcs[20];
static struct mbstests {
	char *s;
	char *sexp;

	wchar_t *wcs;
	wchar_t *wcsexp;

	size_t n;
	int r;
	int syserr;
	int mbstate;
} tests[] = {
	/* s       sexp    wcs  wcsexp                      n  r  syserr  mbstate */
	{"\0",     NULL,   wcs, (wchar_t[]) {0},            1, 0,      0,       1},
	{"\0",      str,   wcs, (wchar_t[]) {0},            0, 0,      0,       1},
	{"\0",      str,  NULL, (wchar_t[]) {0},            1, 0,      0,       1},
	{"\0",      str,  NULL, (wchar_t[]) {0},            0, 0,      0,       1},

	{"\x31",   NULL,   wcs, (wchar_t[]) {0x31, 0},      2, 1,      0,       1},
	{"\x31",  str+1,   wcs, (wchar_t[]) {0x31, 0},      1, 1,      0,       1},
	{"\x31",    str,  NULL, (wchar_t[]) {0x31, 0},      1, 1,      0,       1},
	{"\x31",    str,  NULL, (wchar_t[]) {0x31, 0},      0, 1,      0,       1},

	{"\x21\xc2\xa1\xe2\x80\x94\xf0\x9f\x92\xa9",
               NULL,   wcs,
               (wchar_t[]) {0x21,0xa1,0x2014,0x1f4A9}, 20, 4,      0,       1},

	{"\xf0\x9f",str,  wcs, NULL,                       20,-1, EILSEQ,       0},
};

void
tests_mbsrtowcs(void)
{
	int r;
	const char *s;
	mbstate_t st;
	struct mbstests *tp;

	puts("testing mbsrtowcs1");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		errno = 0;
		if (tp->s != NULL)
			s = strcpy(str, tp->s);			
		memset(wcs, -1, sizeof(wcs));

		r = mbsrtowcs(tp->wcs, &s, tp->n, NULL);
		assert(tp->r == r);
		assert(tp->syserr == errno);
		if (tp->r >= 0) {
			assert(s == tp->sexp);
			if (tp->wcs)
				assert(!wcsncmp(tp->wcsexp, tp->wcs, tp->r));
		}
	}

	puts("testing mbsrtowcs2");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		errno = 0;
		if (tp->s != NULL)
			s = strcpy(str, tp->s);			
		memset(wcs, -1, sizeof(wcs));
		memset(&st, 0, sizeof(st));

		r = mbsrtowcs(tp->wcs, &s, tp->n, &st);
		assert(tp->r == r);
		assert(tp->syserr == errno);
		if (tp->r >= 0) {
			assert(s == tp->sexp);
			if (tp->wcs)
				assert(!wcsncmp(tp->wcsexp, tp->wcs, tp->r));
			assert(mbsinit(&st) != 0 == tp->mbstate);
		}
	}
}

void
tests_mbstowcs(void)
{
	int r;
	struct mbstests *tp;

	puts("testing mbstowcs");
	for (tp = tests; tp < &tests[NELEM(tests)]; ++tp) {
		errno = 0;
		memset(wcs, -1, sizeof(wcs));

		r = mbstowcs(tp->wcs, tp->s, tp->n);
		assert(tp->r == r);
		assert(tp->syserr == errno);
		if (tp->r >= 0) {
			if (tp->wcs)
				assert(!wcsncmp(tp->wcsexp, tp->wcs, tp->r));
		}
	}
}

int
main(void)
{
	puts("testing");
	tests_mbsrtowcs();
	tests_mbstowcs();
	puts("done");
	return 0;
}
