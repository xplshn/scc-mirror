/*
PATTERN:
0038-void.c:18: error: 'void' must be the only parameter
0038-void.c:24: error: 'void' must be the only parameter
0038-void.c:30: error: 'void' must be the only parameter
0038-void.c:45: error: too many arguments in function call
0038-void.c:46: error: too few arguments in function call
0038-void.c:48: error: too many arguments in function call
0038-void.c:49: error: too few arguments in function call
0038-void.c:50: error: too few arguments in function call
0038-void.c:52: error: too few arguments in function call
0038-void.c:54: error: too many arguments in function call
too many errors
.
*/

int
f1(void, int a)
{
	return 0;
}

int
f2(int a, void)
{
	return 0;
}

int
f3(int a, void, int b)
{
	return 0;
}

int
f4(void)
{
	return 0;
}

int
main()
{
	f1(1);
	f1(1, 2);
	f1();
	f2(1);
	f2(1, 2);
	f2();
	f3(1);
	f3(1, 2);
	f3();
	f4();
	return f4(1);
}
