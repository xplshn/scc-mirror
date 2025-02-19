#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
copysec(Mapsec *msec, FILE *fp)
{
	long n;
	Section *sec = &msec->sec;

	fseek(msec->fp, sec->offset, SEEK_SET);
	for (n = msec->fsiz; n != 0; n--)
		putc(getc(msec->fp), fp);

	return ferror(msec->fp) ? -1 : 0;
}
