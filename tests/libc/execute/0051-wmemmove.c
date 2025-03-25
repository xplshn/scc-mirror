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
	wchar_t buf[30];
	wchar_t def[] = {'d', 'e', 'f', '\0'};
	wchar_t abc[] = {'a', 'b', 'c', '\0'};
	wchar_t abcdef[]  = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};

	puts("testing");

	wmemcpy(buf, abcdef, 6);
	assert(!wmemcmp(wmemmove(buf, buf+3, 3), def, 3));
	wmemcpy(buf, abcdef, 6);
	assert(!wmemcmp(wmemmove(buf, buf+3, 0), abc, 3));

	puts("done");

	return 0;
}
