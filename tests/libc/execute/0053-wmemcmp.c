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
	wchar_t buf[40] = {1, 2, 3, 4, 5};

	puts("testing");
	assert(wmemcmp(buf, (wchar_t[]) {1, 2, 3, 4, 5}, 5) == 0);
	assert(wmemcmp(buf, (wchar_t[]) {1, 1, 1, 1, 1}, 5) > 0);
	assert(wmemcmp(buf, (wchar_t[]) {1, 3, 1, 1, 1}, 5) < 0);
	assert(wmemcmp(buf, (wchar_t[]) {2, 3, 4, 5, 6}, 0) == 0);
	puts("done");

	return 0;
}
