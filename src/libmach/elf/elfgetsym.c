#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/elf/elftypes.h>
#include <scc/elf/elfent.h>
#include <scc/elf/elfshdr.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

static int
typeof(Elf *elf, Elfsym *ent, char *name)
{
	int c, bind, weak;
	unsigned long flags, type;
	Elfsec *shdr;

	weak = ELF_ST_BIND(ent->info) == STB_WEAK;

	switch (ent->shndx) {
	case SHN_UNDEF:
		c = 'U';
		break;
	case SHN_ABS:
		c = 'a';
		break;
	case SHN_COMMON:
		c = 'C';
		break;
	case SHN_XINDEX:
		abort();
	default:
		shdr = &elf->secs[ent->shndx];
		flags = shdr->flags;
		type = shdr->type;

		if (ELF_ST_BIND(ent->info) == STB_WEAK) {
			c = (flags & SHF_EXECINSTR) ? 'W' : 'V';
		} else {
			if (flags & SHF_ALLOC) {
				if (type == SHT_NOBITS)
					c = 'b';
				else if (flags & SHF_WRITE)
					c = 'd';
				else if (flags & SHF_EXECINSTR)
					c = 't';
				else
					c = 'r';
			} else if (strncmp(name, ".debug", 6) == 0) {
				c = 'N';
			} else if (strcmp(name, ".comment") == 0) {
				c = 'N';
			} else if (strcmp(name, ".line") == 0) {
				c = 'N';
			} else if (strcmp(name, ".stab") == 0) {
				c = 'N';
			} else {
				c = '?';
			}
			if (ELF_ST_BIND(ent->info) != STB_LOCAL)
				c = toupper(c);
		}
	}

	return c;
}

static int
stypeof(Elfsym *ent)
{
	switch (ELF_ST_TYPE(ent->info)) {
	case STT_OBJECT:
		return SYMOBJECT;
	case STT_FUNC:
		return SYMFUNC;
	case STT_SECTION:
		return SYMSECTION;
	case STT_FILE:
		return SYMFILE;
	case STT_COMMON:
		return SYMCOMMON;
	default:
	case STT_NOTYPE:
		return SYMNOTYPE;
	}
}

#include <assert.h>

Symbol *
elfgetsym(Obj *obj, int *idx, Symbol *sym)
{
	int n = *idx;
	Elfsym *ent;
	Elf *elf = obj->data;

	if (n == 0)
		n++;

	if (!elf->syms || n >= elf->nsym)
		return NULL;
	ent = &elf->syms[n];

	if (ELF_ST_TYPE(ent->info) == STT_SECTION) {
		Elfsec *shdr = &elf->secs[ent->shndx];
		sym->name = shdr->name;
	} else {
		sym->name = ent->name;
	}

	// assert(strlen(sym->name) > 0);
	sym->type = typeof(elf, ent, sym->name);
	sym->stype = stypeof(ent);
	sym->value = ent->value;
	sym->size = ent->size;
	sym->index = *idx = n;

	return sym;
}
