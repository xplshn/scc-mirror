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
main(void)
{
	wchar_t t1[] = {0};
	wchar_t t2[] = {0};
	wchar_t t3[] = {0x31, 0};
	wchar_t t4[] = {0x31, 0};
	wchar_t t5[] = {0x32, 0x33, 0};

	puts("testing");
	assert(wcsncmp(t1, t1, 0) == 0);
	assert(wcsncmp(t1, t2, 0) == 0);
	assert(wcsncmp(t3, t4, 1) == 0);
	assert(wcsncmp(t3, t5, 1) < 0);
	assert(wcsncmp(t5, t3, 1) > 0);
	puts("done");

	return 0;
}
