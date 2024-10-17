/*
PATTERN:
0038-void.c:22: error: 'void' must be the only parameter
0038-void.c:27: warning: 'a' defined but not used
0038-void.c:28: error: 'void' must be the only parameter
0038-void.c:33: warning: 'a' defined but not used
0038-void.c:34: error: 'void' must be the only parameter
0038-void.c:39: warning: 'b' defined but not used
0038-void.c:39: warning: 'a' defined but not used
0038-void.c:49: error: too many arguments in function call
0038-void.c:50: error: too few arguments in function call
0038-void.c:52: error: too many arguments in function call
0038-void.c:53: error: too few arguments in function call
0038-void.c:54: error: too few arguments in function call
0038-void.c:56: error: too few arguments in function call
0038-void.c:58: error: too many arguments in function call
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
