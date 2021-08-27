#include <stdlib.h>

#include "../libc.h"

#undef exit

void (*_exitf[_ATEXIT_MAX])(void);
unsigned _exitn;

void
exit(int status)
{
	while (_exitn > 0)
		(*_exitf[--_exitn])();

	_Exit(status);
}
