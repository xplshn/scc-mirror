#include <assert.h>
#include <stdio.h>
#include <wchar.h>

/*
output:
testing
done
end:
*/

int
main()
{
	wchar_t test[]= {'t', 'e', 's', 't', 0};
	wchar_t *s, buf[40];

	puts("testing");

	s = wcscpy(buf, test);
	assert(s == buf);
	assert(!wcscmp(s, test));

	s = wcscpy(buf, "");
	assert(s == buf);
	assert(!wcscmp(s, ""));

	puts("done");

	return 0;
}