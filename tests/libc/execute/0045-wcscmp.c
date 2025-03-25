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
	wchar_t t5[] = {0x31, 0x32, 0};
	wchar_t t6[] = {0, 0x31, 0};

	puts("testing");
	assert(wcscmp(t1, t1) == 0);
	assert(wcscmp(t1, t2) == 0);
	assert(wcscmp(t1, t3) < 0);
	assert(wcscmp(t3, t1) > 0);
	assert(wcscmp(t3, t3) == 0);
	assert(wcscmp(t3, t4) == 0);
	assert(wcscmp(t3, t5) < 0);
	assert(wcscmp(t5, t3) > 0);
	assert(wcscmp(t3, t6) > 0);
	assert(wcscmp(t6, t3) < 0);
	puts("done");

	return 0;
}
