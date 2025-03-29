#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "objdump.h"

int
coff32hasrelloc(Obj *obj, Section *sec)
{
	struct coff32 *coff = obj->data;
	SCNHDR *scn = &coff->scns[sec->index];

	return scn->s_nrelloc != 0;
}

void
coff32syms(Obj *obj)
{
	long i;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	if (hdr->f_nsyms == 0) {
		puts("no symbols");
		return;
	}

	for (i = 0; i < hdr->f_nsyms; i++) {
		SYMENT *ent;
		AUXENT *aux;
		Entry *ep = &coff->ents[i];

		aux = &ep->u.aux;
		switch (ep->type) {
		case SYM_ENT:
			ent = &ep->u.sym;
			printf("[%4ld](sec %2d)(ty %4x)(scl %3d) (nx %d) 0x%04lX %s\n", 
			       i,
			       ent->n_scnum,
			       ent->n_type,
			       ent->n_sclass,
			       ent->n_numaux,
			       ent->n_value,
			       coff32str(coff, ent));
			break;
		case SYM_AUX_UNK:
			puts("AUX");
			break;
		case SYM_AUX_SYM:
			printf("AUX tagndx %ld lnno %d size 0x%x lnnoptr %ld endndx %ld tv %d\n",
			       aux->x_tagndx,
			       aux->x_lnno,
			       aux->x_size,
			       aux->x_lnnoptr,
			       aux->x_endndx,
			       aux->x_tvndx);
			break;
		case SYM_AUX_FILE:
			printf("File %s\n", coff32str(coff, aux));
			break;
		case SYM_AUX_SCN:
			printf("AUX scnlen 0x%lx nreloc %d nlnno %d chk %lu ass %d comdat %d\n",
			       aux->x_scnlen,
			       aux->x_nreloc,
			       aux->x_nlinno,
			       aux->x_checksum,
			       aux->x_associated,
			       aux->x_comdat);
			break;
		case SYM_AUX_FUN:
			printf("AUX tagndx %ld fsize 0x%lx lnnoptr %ld endndx %ld tv %d\n",
			       aux->x_fsize,
			       aux->x_tagndx,
			       aux->x_lnnoptr,
			       aux->x_endndx,
			       aux->x_tvndx);
			break;
		case SYM_AUX_ARY:
		break;
			abort();
		}
	}
}

void
coff32scns(Obj *obj)
{
	int i;
	char *name;
	SCNHDR *scn;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	for (i = 0; i < hdr->f_nscns; i++) {
		scn = &coff->scns[i];
		name = coff32str(coff, scn);
		if (!selected(name))
			continue;
		printf("\nscnhdr: %d\n"
		       "\tname: %s\n"
		       "\ts_paddr: 0x%04lx\n"
		       "\ts_vaddr: 0x%04lx\n"
		       "\ts_size: 0x%04lx\n"
		       "\ts_scnptr: %ld\n"
		       "\ts_relptr: %ld\n"
		       "\ts_lnnoptr: %ld\n"
		       "\ts_nrelloc: %u\n"
		       "\ts_nlnno: %u\n"
		       "\ts_flags: %#lx\n",
		       i,
		       name,
		       scn->s_paddr,
		       scn->s_vaddr,
		       scn->s_size,
		       scn->s_scnptr,
		       scn->s_relptr,
		       scn->s_lnnoptr,
		       scn->s_nrelloc,
		       scn->s_nlnno,
		       scn->s_flags);
	}	
}

void
coff32fhdr(Obj *obj, unsigned long long *start, Flags *f)
{
	unsigned flags;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;
	AOUTHDR *aout = &coff->aout;

	flags = hdr->f_flags;
	setflag(f, (flags & F_RELFLG) == 0, HAS_RELOC);
	setflag(f, (flags & F_LMNO) == 0, HAS_LINENO);
	setflag(f, (flags & F_LSYMS) == 0, HAS_LOCALS);
	setflag(f, hdr->f_nsyms > 0, HAS_SYMS);
	setflag(f, flags & F_EXEC, EXEC_P);

	if (pflag) {
		printf("FILEHDR:\n"
		       "\tf_magic: %#x\n"
		       "\tf_nscns: %u\n"
		       "\tf_timdat: %ld\n"
		       "\tf_symptr: %ld\n"
		       "\tf_nsyms: %ld\n"
		       "\tf_opthdr: %u\n"
		       "\tf_flags: 0x%04x\n\n",
		       hdr->f_magic,
		       hdr->f_nscns,
		       hdr->f_timdat,
		       hdr->f_symptr,
		       hdr->f_nsyms,
		       hdr->f_opthdr,
		       hdr->f_flags);
	}

	if (hdr->f_opthdr > 0) {
		*start = aout->entry;
		if (pflag) {
			printf("AOUTHDR:\n"
			       "\tmagic: %x\n"
			       "\tvstamp: %x\n"
			       "\ttsize: %04lx\n"
			       "\tdsize: %04lx\n"
			       "\tbsize: %04lx\n"
			       "\tentry: %04lx\n"
			       "\ttext_start: %04lx\n"
		           "\tdata_start: %04lx\n\n",
			       aout->magic,
			       aout->vstamp,
			       aout->tsize,
			       aout->dsize,
			       aout->bsize,
			       aout->entry,
			       aout->text_start,
			       aout->data_start);
		}
	}
}
