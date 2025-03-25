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
	wchar_t buf[] = L"012321";

	puts("testing");
	assert(wcsrchr(buf, '1') == buf+5);
	assert(wcsrchr(buf, '0') == buf);
	assert(wcsrchr(buf, '3') == buf+3);
	assert(wcsrchr("",  '0') == NULL);
	assert(wcsrchr(buf, 'a')  == NULL);
	assert(wcsrchr(buf, 0) == buf+6);
	puts("done");

	return 0;
}
