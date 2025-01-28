#include <stdio.h>

#include <scc/mach.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

Map *
elfloadmap(Obj *obj, FILE *fp)
{
	long i;
	Map *map;
	char *name;
	long nsec;
	FILE *src;
	Elfsec *shdr;
	Elf *elf = obj->data;
	Elfhdr *hdr = &elf->hdr;

	nsec = hdr->shnum;
	if ((map = newmap(NULL, nsec)) == NULL)
		return NULL;

	for (shdr = elf->secs; nsec--; ++shdr) {
		unsigned long o;
		unsigned long long b = shdr->addralign;
		unsigned long long e = b + shdr->size;

		if (shdr->offset != 0) {
			o = obj->pos + shdr->offset;
			src = fp;
		} else {
			o = 0;
			src = NULL;
		}

		setmap(map, name, src, b, e, o);
	}

	return map;
}
