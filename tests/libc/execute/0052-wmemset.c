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
	wchar_t *p, buf[40];

	puts("testing");

	wmemset(buf, 2, 40);
	for (p = buf; p < &buf[40]; ++p)
		assert(*p == 2);

	wmemset(buf, 0, 0);
	for (p = buf; p < &buf[40]; ++p)
		assert(*p == 2);

	puts("done");

	return 0;
}
