#include <stdio.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

Map *
elfloadmap(Obj *obj, FILE *fp)
{
	int i;
	int nsec, nseg;
	Map *map;
	Section sec;
	FILE *src;
	Elf *elf = obj->data;
	Elfhdr *hdr = &elf->hdr;
	Elfphdr *phdr;

	nseg = hdr->phnum;
	nsec = elf->nsec;
	if ((map = newmap(nsec, nseg)) == NULL)
		return NULL;

	memset(&sec, 0, sizeof(sec));
	for (i = 0; i < nseg; ++i) {
		phdr = &elf->phdr[i];
		sec.name = NULL;
		sec.load = phdr->vaddr;
		sec.base = phdr->paddr;
		sec.offset = obj->pos + phdr->offset;
		sec.align = phdr->align;
		sec.size = phdr->memsz;
		sec.index = i;

		if (mapseg(map, &sec, fp, phdr->filesz) < 0)
			return NULL;
	}

	for (i = 0; getsec(obj, &i, &sec); ++i) {
		sec.offset += obj->pos;
		src = ((sec.flags & SALLOC) != 0) ? fp : NULL;

		if (mapsec(map, &sec, src, sec.size) < 0)
			return NULL;
	}

	return map;
}
