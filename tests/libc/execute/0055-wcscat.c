#include <assert.h>
#include <stdio.h>
#include <wchar.h>

/*
output:
testing
ok
end:
*/

int
main(void)
{
	wchar_t *s, buf[40];

	puts("testing");
	wcscpy(buf, L"case1:");
	s = wcscat(buf, L"ok");
	assert(s == buf);
	assert(!wcscmp(s, L"case1:ok"));

	wcscpy(buf, L"");
	s = wcscat(buf, L"ok");
	assert(s == buf);
	assert(!wcscmp(s, L"ok"));

	wcscpy(buf, L"case1:");
	wcscat(buf, L"");
	assert(s == buf);
	assert(!wcscmp(s, L"case1:"));

	puts("ok");

	return 0;
}
