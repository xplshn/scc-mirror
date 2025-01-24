#include <stdio.h>

#include <scc/mach.h>
#include <scc/elf64.h>

#include "../libmach.h"

Section *
elf64getsec(Obj *obj, int *idx, Section *sec)
{
	int stype, n = *idx;
	unsigned long flags, type;
	unsigned sflags;
	Elf64 *elf = obj->data;
	Elf_Ehdr *hdr = &elf->hdr;
	Elf_Shdr *shdr;

	if (n >= elf->nsec) {
		if (n == SHN_ABS)
			sec->name = "*ABS";
		else if (n == SHN_COMMON)
			sec->name = "*COM*";
		else
			sec->name = "*UNK*";
		return NULL;
	}

	shdr = &elf->shdr[n];
	flags = shdr->sh_flags;
	type = shdr->sh_type;

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
		sec->name = elf64str(obj, SEC_STRTBL, shdr->sh_name);

	sec->index = n;
	sec->size = shdr->sh_size;
	sec->base = shdr->sh_addr;
	sec->load = shdr->sh_addr;
	sec->offset = shdr->sh_offset;
	sec->type = stype;
	sec->flags = sflags;
	sec->align = shdr->sh_addralign;

	return sec;
}
