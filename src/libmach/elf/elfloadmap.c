#include <stdio.h>

#include <scc/mach.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

Map *
elfloadmap(Obj *obj, FILE *fp)
{
	int nsec, nseg;
	unsigned long o, s;
	unsigned long long b, e;

	Map *map;
	FILE *src;
	Elfsec *shdr;
	Elf *elf = obj->data;
	Elfhdr *hdr = &elf->hdr;
	Elfphdr *phdr;

	nseg = hdr->phnum;
	nsec = elf->nsec;
	if ((map = newmap(nsec, nseg)) == NULL)
		return NULL;

	for (shdr = elf->secs; nsec-- > 0; ++shdr) {
		b = shdr->addr;
		e = b + shdr->size;

		if (shdr->offset != 0) {
			s = shdr->size;
			o = obj->pos + shdr->offset;
			src = fp;
		} else {
			s = o = 0;
			src = NULL;
		}

		if (mapsec(map, shdr->name, src, b, e, s, o) < 0)
			return NULL;
	}

	for (phdr = elf->phdr; nseg-- > 0; ++phdr) {
		b = phdr->vaddr;
		e = b + phdr->memsz;

		if (phdr->offset != 0) {
			s = phdr->filesz;
			o = obj->pos + phdr->offset;
			src = fp;
		} else {
			s = o = 0;
			src = NULL;
		}

		if (mapseg(map, NULL, src, b, e, s, o) < 0)
			return NULL;
	}

	return map;
}
