#include <stdio.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

Section *
coff32getsec(Obj *obj, int *idx, Section *sec)
{
	long n = *idx;
	int type;
	unsigned sflags;
	SCNHDR *scn;
	Coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	if (n >= hdr->f_nscns)
		return NULL;

	scn = &coff->scns[n];
	switch (scn->s_flags) {
	case STYP_REG:
		type = 'D';
		sflags = SALLOC | SRELOC | SLOAD | SWRITE | SREAD;
		break;
	case STYP_DSECT:
		type = '?';
		sflags = SRELOC;
		break;
	case STYP_NOLOAD:
		type = 'D';
		sflags = SALLOC | SREAD | SWRITE;
		break;
	case TYP_REVERSE_PAD:
	case STYP_PAD:
		type = '?';
		sflags = SLOAD;
		break;
	case STYP_COPY:
		type = '?';
		sflags |= SLOAD | SRELOC;
		break;
	case STYP_TEXT:
		type = 'T';
		sflags = SALLOC | SRELOC | SLOAD | SEXEC | SREAD | SWRITE;
		break;
	case STYP_DATA:
		type = 'D';
		sflags = SALLOC | SRELOC | SLOAD | SWRITE | SREAD;
		break;
	case STYP_BSS:
		type = 'B';
		sflags = SALLOC | SREAD | SWRITE;
		break;
	case STYP_TEXT | STYP_DATA:
		type = 'D';
		sflags = SEXEC | SALLOC | SRELOC | SLOAD | SREAD | SWRITE;
		break;
	case STYP_LIT:
	case STYP_RDATA:
		type = 'D';
		sflags = SALLOC | SRELOC | SLOAD | SREAD;
		break;
	case STYP_LIB:
	case STYP_INFO:
		type = 'N';
		sflags = 0;
		break;
	case STYP_OVER:
		type = '?';
		sflags = SRELOC;
		break;
	case STYP_GROUP:
	case STYP_MERGE:
	default:
		type = '?';
		sflags = 0;
		break;
	}

	sec->name = coff32str(coff, scn);
	sec->index = n;
	sec->size = scn->s_size;
	sec->load = scn->s_vaddr;
	sec->base = scn->s_paddr;
	sec->offset = scn->s_scnptr;
	sec->type = type;
	sec->flags = sflags;
	sec->align = 4;
	sec->fill = 0;

	return sec;
}
