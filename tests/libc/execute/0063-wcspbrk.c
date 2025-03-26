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
	wchar_t buf[] = L"abcdef";

	puts("testing");
	assert(wcspbrk(buf, L"a") == buf);
	assert(wcspbrk(buf, L"") == NULL);
	assert(wcspbrk(L"", L"a") == NULL);
	assert(wcspbrk(L"", L"") == NULL);
	assert(wcspbrk(buf, L"1234") == NULL);
	assert(wcspbrk(buf, L"f") == buf + 5);
	assert(wcspbrk(buf, L"12345a") == buf);
	puts("done");

	return 0;
}
