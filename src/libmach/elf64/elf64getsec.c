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

	if (n >= elf->nsec)
		return NULL;

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
		sflags |= SALLOC|SREAD;
	if (type != SHT_NOBITS)
		sflags |= SLOAD;
	if (stype == 'T' || stype == 'D')
		sflags |= SRELOC;

	/*
	 * We cannot differentiate between load and base address
	 * in a section, while we can use the physical address
	 * for that when dealing with segments.
	 * Also, we don't have an easy way to know the number of
	 * relocations affecting one section. To know that, we
	 * have to run over the relocation sections and find one
	 * with the sh_link pointing to this section. Maybe,
	 * we should just remove the nrelloc field.
	 */
	sec->name = elf64str(obj, SEC_STRTBL, shdr->sh_name);
	sec->index = n;
	sec->size = shdr->sh_size;
	sec->base = shdr->sh_addr;
	sec->load = shdr->sh_addr;
	sec->offset = shdr->sh_offset;
	sec->nreloc = 0;
	sec->type = stype;
	sec->flags = sflags;
	sec->align = shdr->sh_addralign;

	return sec;
}
