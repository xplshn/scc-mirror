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

	assert(wcscspn(L"0125", L"56789") == 3);
	assert(wcscspn(L"", L"56789") == 0);
	assert(wcscspn(L"01234", L"") == 5);
	assert(wcscspn(L"", L"") == 0);

	puts("done");

	return 0;
}
