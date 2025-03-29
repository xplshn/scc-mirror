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
	wchar_t buf[40];
	size_t n;

	puts("testing");

	assert(wcsxfrm(buf, L"test", 40) == 4);
	assert(wcsxfrm(buf, L"", 0) == 0);
	assert(wcsxfrm(NULL, L"abc", 0) > 0);

	puts("done");

	return 0;
}
