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
	wchar_t buf[] = {0, 1, 2, 3, 4, 160};

	puts("testing");
	assert(wmemchr(buf, 2, 6) == buf+2);
	assert(wmemchr(buf, 2, 0) == NULL);
	assert(wmemchr(buf, 150, 6) == NULL);
	assert(wmemchr(buf, 160, 6) == buf+5);
	puts("done");

	return 0;
}
