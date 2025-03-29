#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

void
elfdel(Obj *obj)
{
        struct elf *elf = obj->data;

        if (elf) {
                free(elf->phdr);
                free(elf->secs);
		free(elf->syms);
		free(elf->rels);
        }
        free(obj->data);
        obj->data = NULL;
}
