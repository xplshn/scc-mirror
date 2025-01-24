#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

static int
typeof(Coff32 *coff, SYMENT *ent)
{
	int c;
	SCNHDR *scn;

	switch (ent->n_scnum) {
	case N_DEBUG:
		c = 'N';
		break;
	case N_ABS:
		c = 'a';
		break;
	case N_UNDEF:
		c = (ent->n_value != 0) ? 'C' : 'U';
		break;
	default:
		scn = &coff->scns[ent->n_scnum-1];

		switch (scn->s_flags) {
		case STYP_TEXT:
			c = 't';
			break;
		case STYP_DATA:
			c = 'd';
			break;
		case STYP_BSS:
			c = 'b';
			break;
		case STYP_INFO:
			c = 'N';
			break;
		case STYP_LIT:
			c = 'r';
			break;
		default:
			c = '?';
			break;
		}
	}

	if (ent->n_sclass == C_EXT)
		c = toupper(c);

	return c;
}

Symbol *
coff32getsym(Obj *obj, int *idx, Symbol *sym)
{
	int n = *idx;
	Entry *ep;
	SYMENT *ent;
	Coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	if ((hdr->f_flags & F_LSYMS) != 0 || n >= coff->hdr.f_nsyms)
		return NULL;

	ep = &coff->ents[n];
	if (ep->type != SYM_ENT)
		return NULL;
	ent = &ep->u.sym;

	sym->name = coff32str(coff, ent);
	sym->type = typeof(coff, ent);
	sym->stype = SYMOBJECT;
	sym->value = ent->n_value;
	sym->size = (sym->type == 'C') ? ent->n_value : 0;
	sym->index = n;
	*idx += ent->n_numaux;

	return sym;
}
