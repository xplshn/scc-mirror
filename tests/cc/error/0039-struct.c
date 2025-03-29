/*
PATTERN:
0039-struct.c:11: warning: type defaults to 'int' in declaration
0039-struct.c:11: warning: empty declaration
0039-struct.c:11: error: expected ';' before '}'
0039-struct.c:19: warning: 'cap' defined but not used
0039-struct.c:19: warning: 'v' defined but not used
.
*/

typedef struct  { } Vec;

static void
vecresize(Vec *v, int cap)
{
	return;
}

int main()
{
	return 0;
}
