#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

/*
output:
testing
test1
test2
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

void
test2(void)
{
	int i;

	printf("test2\n");
#ifdef EUNKNOWN
	for (i = 1; i <= EUNKNOWN; i++) {
		if (!strerror(i))
			printf("invalid string for errno=%d\n", i);
	}
#endif
}

int
main()
{
	printf("testing\n");
	test1();
	test2();
	printf("done\n");

	return 0;
}
