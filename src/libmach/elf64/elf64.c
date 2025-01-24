#include <stdio.h>

#include <scc/mach.h>
#include <scc/elf64.h>

#include "../libmach.h"
#include "fun.h"

char *
elf64str(Obj *obj, int n, long stroff)
{
	char *tbl;
	size_t siz;
	struct elf64 *elf;

	elf = obj->data;

	tbl = elf->strtbl[n];
	siz = elf->strsiz[n];

	if (!tbl || siz < stroff)
		return "";
	return &tbl[stroff];
}
