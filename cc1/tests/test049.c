/*
name: TEST049
description: Basic test for initializer
error:

output:
G1	I	x
(
	#I5
)
G3	P	p
(
	G1	'P
)
F4	I	E
G5	F4	main
{
\
	j	L6	G3	@I	#I5	=I
	r	#I1
L6
	r	#I0
}

*/

int x = 5;
int *p = &x;

int
main()
{
	if(*p != 5)
		return 1;
	return 0;
}
