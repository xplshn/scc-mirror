#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

static char *
secname(Coff32 *coff, SCNHDR *scn, Section *sec)
{
	char *p;
	unsigned long siz = strlen(sec->name);

	if (siz < SCNNMLEN)
		return strncpy(scn->s_name, sec->name, SCNNMLEN);

	if (coff->strsiz > ULONG_MAX - siz - 1)
		return NULL;

	siz += coff->strsiz + 1;
	if ((p = realloc(coff->strtbl, siz)) == NULL)
		return NULL;
	coff->strtbl = p;

	scn->s_zeroes = 0;
	scn->s_offset = coff->strsiz;
	coff->strsiz += siz;
	return strcpy(&coff->strtbl[scn->s_offset], sec->name);
}

Section *
coff32setsec(Obj *obj, int *idx, Section *sec)
{
	long flags, n = *idx;
	SCNHDR *scn;
	Coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	/* TODO: what happens with SABS? */
	switch (sec->type) {
	case 'D':
		switch (sec->flags) {
		case SALLOC | SRELOC | SLOAD | SWRITE | SREAD:
			if (strcmp(sec->name ,".data") == 0)
				flags = STYP_DATA;
			else
				flags = STYP_REG;
			break;
		case SALLOC | SREAD | SWRITE:
			flags = STYP_NOLOAD;
			break;
		case SALLOC | SRELOC | SLOAD | SREAD:
			flags = STYP_RDATA;
			break;
		case SEXEC | SALLOC | SRELOC | SLOAD | SREAD:
			flags = STYP_TEXT | STYP_DATA;
			break;
		default:
			goto invalid;
		}
		break;
	case 'T':
		flags = STYP_TEXT;
		break;
	case 'B':
		flags = STYP_BSS;
		break;
	case 'N':
	case '?':
	default:
	invalid:
		/* TODO */
		return NULL;
	}

	if (strlen(sec->name) >= SCNNMLEN)
		return NULL;

	if (n >= hdr->f_nscns) {
		if (n > SHRT_MAX - 1)
			return NULL;
		scn = realloc(coff->scns, (n+1) * sizeof(SCNHDR));
		if (!scn)
			return NULL;
		coff->scns = scn;
		hdr->f_nscns = n + 1;
	}

	scn = &coff->scns[n];
	if (!secname(coff, scn, sec))
		return NULL;

	/*
	 * sec->offset is ignored because it is very unlikely
	 * that the vaule is meaningful here. The field offset
	 * was added only to allow getsec to retrive the information
	 * in a generic way, but it was never expected to be a 2 way
	 * relation. That pointer is updated when the object file
	 * is written using a mapping.
	 */
	scn->s_paddr = sec->load;
	scn->s_vaddr = sec->base;
	scn->s_size = sec->size;
	scn->s_scnptr = 0;
	scn->s_relptr = 0;
	scn->s_lnnoptr = 0;
	scn->s_nrelloc = 0;
	scn->s_nlnno = 0;
	scn->s_flags = flags; 

	return sec;
}
