#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

/*
output:
testing
done
end:
*/

void
tests_mbsinit(void)
{
	mbstate_t s;

	assert(mbsinit(NULL) != 0);
	assert(mbsinit(memset(&s, 0, sizeof(s))) != 0);
}

int
main()
{
	puts("testing");
	tests_mbsinit();
	puts("done");
	return 0;
}
