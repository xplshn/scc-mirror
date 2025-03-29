#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

/*
output:
testing
test1
done
end:
*/

void
test1(void)
{
	printf("test1\n");
	if (!strerror(EDOM))
		printf("invalid EDOM string\n");
	if (!strerror(ERANGE))
		printf("invalid ERANGE string\n");
	if (!strerror(EILSEQ))
		printf("invalid EILSEQ string\n");
}

int
main()
{
	printf("testing\n");
	test1();
	printf("done\n");

	return 0;
}
