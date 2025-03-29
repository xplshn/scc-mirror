#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/*
output:
testing
test 1
test 2
done
end:
*/

void
vtest(char *fmt, va_list va)
{
	int cnt = 1;
	long long *p;

	while (*fmt) {
		switch (*fmt++) {
		case 'c':
			assert(va_arg(va, int) == cnt++);
			break;
		case 's':
			assert(va_arg(va, int) == cnt++);
			break;
		case 'i':
			assert(va_arg(va, int) == cnt++);
			break;
		case 'l':
			assert(va_arg(va, long) == cnt++);
			break;
		case 'q':
			assert(va_arg(va, long long) == cnt++);
			break;
		case 'p':
			p = va_arg(va, void *);
			assert(*p == cnt++);
			break;
		case 'f':
			assert(va_arg(va, double) == cnt++);
			break;
		default:
			abort();
		}
	}
}

void
test(char *s, char *fmt, ...)
{
	va_list va, vc;

	puts(s);
	va_start(va, fmt);
	va_copy(vc, va);
	vtest(fmt, vc);
	va_end(vc);
	va_end(va);
}

int
main()
{
	char c;
	short s;
	int i;
	long l;
	long long ll;
	float f;

	puts("testing");
	c = 1;
	i = 2;
	l = 3;
	ll = 4;
	test("test 1", "cilp", c, i, l, (void *) &ll);

	c = 1;
	s = 2;
	ll = 3;
	f = 4.0;
	test("test 2", "csqf", c, s, ll, f);
	puts("done");

	return 0;
}
