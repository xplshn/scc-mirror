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
	wchar_t buf[30] = L"abc";

	puts("testing");
	assert(wcsstr(buf, L"abc") == buf);
	assert(wcsstr(buf, L"bc") == buf + 1);
	assert(wcsstr(buf, L"c") == buf + 2);
	assert(wcsstr(buf, L"d") == NULL);
	wcscpy(buf, L"ababc");
	assert(wcsstr(buf, L"abc") == buf+2);
	assert(wcsstr(L"", L"abc") == NULL);
	assert(wcsstr(buf, L"") == buf);
	buf[0] = '\0';
	assert(wcsstr(buf, L"") == buf);
	puts("done");

	return 0;
}

