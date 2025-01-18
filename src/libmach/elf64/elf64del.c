#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>
#include <scc/elf64.h>

#include "../libmach.h"

void
elf64del(Obj *obj)
{
        struct elf64 *elf = obj->data;

        if (elf) {
                free(elf->phdr);
                free(elf->shdr);
		free(elf->syms);
                free(elf->strtbl[0]);
                free(elf->strtbl[1]);
        }
        free(obj->data);
        obj->data = NULL;
}
