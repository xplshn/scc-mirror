#include <assert.h>
#include <stdio.h>
#include <wchar.h>

#define SIZ 6

/*
output:
testing
test1
test2
test3
done
end:
*/

int
main()
{
	wchar_t *s, buf[SIZ];
	wchar_t abc[] = {'a', 'b', 'c'};

	puts("testing");

	puts("test1");
	wmemset(buf, '0', SIZ);
	s = wcsncpy(buf, abc, SIZ);
	assert(s == buf);
	assert(!wmemcmp(s, abc, SIZ));

	puts("test2");
	wmemset(buf, '0', SIZ);
	s = wcsncpy(buf, (wchar_t[]) {0}, SIZ);
	assert(s == buf);
	assert(!wmemcmp(s, (wchar_t[SIZ]) {0}, SIZ));

	puts("test3");
	wmemset(buf, '0', SIZ);
	s = wcsncpy(buf, (wchar_t[]) {0}, 1);
	assert(s == buf);
	assert(!wmemcmp(s,
	                (wchar_t[SIZ]) {'\0', '0', '0', '0', '0', '0'},
	                SIZ));

	puts("done");

	return 0;
}
