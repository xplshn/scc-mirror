/*
PATTERN:
0018-voidparam.c:12: error: 'void' must be the only parameter
0018-voidparam.c:18: error: 'void' must be the only parameter
0018-voidparam.c:30: error: a named argument is requiered before '...'
0018-voidparam.c:30: error: 'void' must be the only parameter
.
*/


int
a(void, int i)
{
	return i;
}

int
b(int i, void)
{
	return i;
}

int
c(void, void)
{
	return 0;
}

int
d(void, ...)
{
	return 0;
}

int
main()
{
	return 0;
}
