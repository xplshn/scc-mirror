#include <stdio.h>

#include <scc/mach.h>
#include <scc/elf/elftypes.h>
#include <scc/elf/elfshdr.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

Section *
elfgetsec(Obj *obj, int *idx, Section *sec)
{
	int stype, n = *idx;
	unsigned long flags, type;
	unsigned sflags;
	Elf *elf = obj->data;
	Elfsec *shdr;

	if (n >= elf->nsec) {
		if (n == SHN_ABS)
			sec->name = "*ABS";
		else if (n == SHN_COMMON)
			sec->name = "*COM*";
		else
			sec->name = "*UNK*";
		return NULL;
	}

	shdr = &elf->secs[n];
	flags = shdr->flags;
	type = shdr->type;

	if (flags & SHF_ALLOC) {
		if (type == SHT_NOBITS)
			stype = 'B';
		else if (flags & SHF_WRITE)
			stype = 'D';
		else
			stype = 'T';
	} else {
		stype = 'N';
	}

	sflags = 0;
	if (flags & SHF_WRITE)
		sflags |= SWRITE;
	if (flags  & SHF_EXECINSTR)
		sflags |= SEXEC;
	if (flags & SHF_ALLOC)
		sflags |= SLOAD|SREAD;
	if (type != SHT_NOBITS)
		sflags |= SALLOC;
	if (stype == 'T' || stype == 'D')
		sflags |= SRELOC;

	/*
	 * We cannot differentiate between load and base address
	 * in a section, while we can use the physical address
	 * for that when dealing with segments.
	 */
	if (n == SHN_UNDEF)
		sec->name = "*UND*";
	else
		sec->name = shdr->name;

	sec->index = n;
	sec->size = shdr->size;
	sec->base = shdr->addr;
	sec->load = shdr->addr;
	sec->offset = shdr->offset;
	sec->type = stype;
	sec->flags = sflags;
	sec->align = shdr->addralign;

	return sec;
}
