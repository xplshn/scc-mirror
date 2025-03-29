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
	puts("testing");
	assert(wcsspn(L"abccdef", L"cba") == 4);
	assert(wcsspn(L"abcg", L"cba0") == 3);
	assert(wcsspn(L"", L"abc") == 0);
	assert(wcsspn(L"abc", L"") == 0);
	assert(wcsspn(L"", L"") == 0);
	assert(wcsspn(L"abc", L"def") == 0);
	puts("done");

	return 0;
}
