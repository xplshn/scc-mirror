#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
output:
testing
done
end:
*/

int
main(void)
{
	wchar_t t1[] = {0, 0};
	wchar_t t2[] = {0};
	wchar_t t3[] = {0x31, 0};
	wchar_t t4[] = {0x31, 0x32, 0, 0x33, 0};

	puts("testing");
	assert(wcslen(t1) == 0);
	assert(wcslen(t2) == 0);
	assert(wcslen(t3) == 1);
	assert(wcslen(t4) == 2);
	puts("done");

	return 0;
}
