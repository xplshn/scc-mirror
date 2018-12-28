#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#undef strtoll

long long
strtoll(const char *s, char **end, int base)
{
	int d, sign = -1;
	long long n;
	static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char *t, *p;

	if (end)
		*end = s;

	while (isspace(*s))
		++s;

	switch (*s) {
	case '-':
		sign = 1;
	case '+':
		++s;
	}

	if (base == 0) {
		if (*s == '0')
			base = toupper(s[1]) == 'X' ? 16 : 8;
		else
			base = 10;
	}
	if (base == 16 && *s == '0' && toupper(s[1]) == 'X')
		s += 2;

	n = 0;
	/* Compute n as a negative number to avoid overflow on LLONG_MIN */
	for (t = s; p = strchr(digits, toupper(*t)); ++t) {
		if ((d = p - digits) >= base)
			break;
		if (n < LLONG_MIN/base)
			goto overflow;
		n *= base;
		if (-d < LLONG_MIN - n)
			goto overflow;
		n -= d;
	}

	if (n == LLONG_MIN && sign < 0) {
		--t;
		goto overflow;
	}

	if (end && t != s)
		*end = t;

	return n*sign;

overflow:
	if (end)
		*end = t;
	errno = ERANGE;

	return sign < 0 ? LLONG_MAX : LLONG_MIN;
}
