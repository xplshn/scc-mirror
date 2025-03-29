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
	wchar_t abc[] = {'a', 'b', 'c', '\0'};

	puts("testing");
	assert(!wmemcmp(wmemcpy(buf, abc, 3), abc, 3));
	assert(wmemcpy(buf, abc, 0) == buf);
	puts("done");

	return 0;
}
