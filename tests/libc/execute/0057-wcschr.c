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
	wchar_t *p, buf[] = L"abcad";

	puts("testing");

	p = wcschr(buf, 0);
	assert(p == buf+5);
	assert(*p == '\0');

	p = wcschr(buf, 'a');
	assert(p == buf);
	assert(*p == 'a');

	p = wcschr(buf, 'd');
	assert(p == buf+4);
	assert(*p == 'd');

	p = wcschr(buf, 'c');
	assert(p == buf+2);
	assert(*p == 'c');

	p = wcschr(buf, 'h');
	assert(p == NULL);

	p = wcschr("", 'a');
	assert(p == NULL);

	puts("done");

	return 0;
}
