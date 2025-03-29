#include <assert.h>
#include <stdio.h>
#include <wchar.h>

/*
output:
testing
test1
test2
test3
test4
test5
done
end:
*/

int
main()
{
	wchar_t *s, buf[40], buf2[40];

	puts("testing");

	puts("test1");
	wcscpy(buf, L"01234");
	s = wcsncat(buf, L"567", 8);
	assert(s == buf);
	assert(!wcscmp(s, L"01234567"));

	puts("test2");
	wcscpy(buf, L"01234");
	s = wcsncat(buf, L"567", 2);
	assert(s == buf);
	assert(!wcscmp(s, L"0123456"));

	puts("test3");
	wcscpy(buf, L"01234");
	wmemcpy(buf2, L"567", 3);
	s = wcsncat(buf, buf2, 3);
	assert(s == buf);
	assert(!wcscmp(s, L"01234567"));

	puts("test4");
	wcscpy(buf, L"01234");
	s = wcsncat(buf, L"", 7);
	assert(s == buf);
	assert(!wcscmp(s, L"01234"));

	puts("test5");
	wcscpy(buf, L"01234");
	s = wcsncat(buf, L"", 0);
	assert(s == buf);
	assert(!wcscmp(s, L"01234"));

	puts("done");

	return 0;
}
