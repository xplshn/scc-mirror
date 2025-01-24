#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

static void
unpack_hdr(int order, unsigned char *buf, FILHDR *hdr)
{
	int n;

	n = unpack(order,
	           buf,
	           "sslllss",
	           &hdr->f_magic,
	           &hdr->f_nscns,
	           &hdr->f_timdat,
	           &hdr->f_symptr,
	           &hdr->f_nsyms,
	           &hdr->f_opthdr,
	           &hdr->f_flags);
	assert(n == FILHSZ);
}

static void
unpack_line(int order, unsigned char *buf, LINENO *lp)
{
	int n;

	n = unpack(order,
	           buf,
	           "ls",
	           &lp->l_addr.l_symndx,
	           &lp->l_lnno);
	assert(n == LINESZ);
}

static void
unpack_scn(int order, unsigned char *buf, SCNHDR *scn)
{
	int n;
	char *s;

	n = unpack(order,
	          buf,
	          "'8llllllssl",
	          scn->s_name,
	          &scn->s_paddr,
	          &scn->s_vaddr,
	          &scn->s_size,
	          &scn->s_scnptr,
	          &scn->s_relptr,
	          &scn->s_lnnoptr,
	          &scn->s_nrelloc,
	          &scn->s_nlnno,
	          &scn->s_flags);
	assert(n == SCNHSZ);

	s = scn->s_name;
	if (!s[0] && !s[1] && !s[2] && !s[3])
		unpack(order, buf, "ll", &scn->s_zeroes, &scn->s_offset);
}

static void
unpack_ent(int order, unsigned char *buf, SYMENT *ent)
{
	int n;
	char *s;

	n = unpack(order,
	           buf,
	           "'8lsscc",
	           ent->n_name,
	           &ent->n_value,
	           &ent->n_scnum,
	           &ent->n_type,
	           &ent->n_sclass,
	           &ent->n_numaux);
	assert(n == SYMESZ);

	s = ent->n_name;
	if (!s[0] && !s[1] && !s[2] && !s[3])
		unpack(order, buf, "ll", &ent->n_zeroes, &ent->n_offset);
}

static void
unpack_reloc(int order, unsigned char *buf, RELOC *rel)
{
	int n;

	n = unpack(order,
	           buf,
	           "lls",
	           &rel->r_vaddr,
	           &rel->r_symndx,
	           &rel->r_type);
	assert(n == RELSZ);
}

static void
unpack_aout(int order, unsigned char *buf, AOUTHDR *aout)
{
	int n;

	n = unpack(order,
	           buf,
	           "ssllllll",
	           &aout->magic,
	           &aout->vstamp,
	           &aout->tsize,
	           &aout->dsize,
	           &aout->bsize,
	           &aout->entry,
	           &aout->text_start,
	           &aout->data_start);
	assert(n == AOUTSZ);
}

static void
unpack_aux_file(int order, unsigned char *buf, AUXENT *aux)
{
	int n;
	char *s;

	n = unpack(order,
	           buf,
	           "'18",
	           aux->x_fname);
	assert(n == AUXESZ);

	s = aux->x_fname;
	if (!s[0] && !s[1] && !s[2] && !s[3])
		unpack(order, buf, "ll", &aux->x_zeroes, &aux->x_offset);
}

static void
unpack_aux_scn(int order, unsigned char *buf, AUXENT *aux)
{
	char dummy1, dummy2, dummy3;
	int n;

	n = unpack(order,
	           buf,
	           "lsslscccc",
	           &aux->x_scnlen,
	           &aux->x_nreloc,
	           &aux->x_nlinno,
	           &aux->x_checksum,
	           &aux->x_associated,
	           &aux->x_comdat,
	           &dummy1,
	           &dummy2,
	           &dummy3);
	assert(n == AUXESZ);
}

static void
unpack_aux_fun(int order, unsigned char *buf, AUXENT *aux)
{
	int n;

	n = unpack(order,
	           buf,
	           "lllls",
	           &aux->x_tagndx,
	           &aux->x_fsize,
	           &aux->x_lnnoptr,
	           &aux->x_endndx,
	           &aux->x_tvndx);
	assert(n == AUXESZ);
}

static void
unpack_aux_ary(int order, unsigned char *buf, AUXENT *aux)
{
	int n;

	n = unpack(order,
	           buf,
	           "lssssss",
	           &aux->x_tagndx,
	           &aux->x_lnno,
	           &aux->x_size,
	           &aux->x_dimen[0],
	           &aux->x_dimen[1],
	           &aux->x_dimen[2],
	           &aux->x_dimen[3],
	           &aux->x_tvndx);
	assert(n == AUXESZ);
}

static void
unpack_aux_sym(int order, unsigned char *buf, AUXENT *aux)
{
	int n;

	n = unpack(order,
	           buf,
	           "lsslls",
	           &aux->x_tagndx,
	           &aux->x_lnno,
	           &aux->x_size,
	           &aux->x_lnnoptr,
	           &aux->x_endndx,
	           &aux->x_tvndx);
	assert(n == AUXESZ);
}

static int
readhdr(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[FILHSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (fread(buf, FILHSZ, 1, fp) != 1)
		return 0;
	unpack_hdr(ORDER(obj->type), buf, hdr);

	if (hdr->f_nsyms < 0 || hdr->f_nscns < 0)
		return 0;

	return 1;
}

static int
readstr(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	long siz;
	char *str;
	unsigned char buf[10];

	coff  = obj->data;
	hdr = &coff->hdr;

	coff->strsiz = 0;
	coff->strtbl = NULL;

	if (hdr->f_nsyms == 0)
		return 1;

	if (fread(buf, 4, 1, fp) != 1)
		return 0;
	unpack(ORDER(obj->type), buf, "l", &siz);
	if (siz < 4 || siz > SIZE_MAX)
		return 0;
	if (siz == 4)
		return 1;

	if ((str = malloc(siz)) == NULL)
		return 0;
	coff->strtbl = str;
	coff->strsiz = siz;

	return fread(str+4, siz-4, 1, fp) == 1;
}

static int
readreloc(Obj *obj, FILE *fp)
{
	int i;
	long j;
	RELOC **rels, *rp;
	SCNHDR *scn;
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[RELSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_nscns == 0)
		return 1;

	rels = calloc(hdr->f_nscns, sizeof(*rels));
	if (!rels)
		return 0;
	coff->rels = rels;

	for (i = 0; i < hdr->f_nscns; i++) {
		scn = &coff->scns[i];
		if (scn->s_nrelloc == 0)
			continue;

		if (!objpos(obj, fp, scn->s_relptr))
			return 0;

		rp = calloc(scn->s_nrelloc, sizeof(RELOC));
		if (!rp)
			return 0;
		rels[i] = rp;

		for (j = 0; j < scn->s_nrelloc; j++) {
			if (fread(buf, RELSZ, 1, fp) != 1)
				return 0;
			unpack_reloc(ORDER(obj->type), buf, &rp[i]);
			if (rp[i].r_symndx >= hdr->f_nsyms)
				return 0;
		}
	}

	return 1;
}

static int
readauxs(Obj *obj, FILE *fp, SYMENT *ent, long pos)
{
	int i, n, typ, d1;
	Entry *ep;
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[AUXESZ];

	if ((n = ent->n_numaux) == 0)
		return 1;
	if (fread(buf, sizeof(buf), 1, fp) != 1)
		return 0;

	coff  = obj->data;
	ep = &coff->ents[pos+1];
	typ = ent->n_type & 15;
	d1 = (ent->n_type >> 4) & 15;

	switch (ent->n_sclass) {
	case C_FILE:
		ep->type = SYM_AUX_FILE;
		unpack_aux_file(ORDER(obj->type), buf, &ep->u.aux);
		break;
	case C_STAT:
		if (d1 == DT_NON && typ == T_NULL) {
			ep->type = SYM_AUX_SCN;
			unpack_aux_scn(ORDER(obj->type), buf, &ep->u.aux);
			break;
		}
	default:
		switch (d1) {
		case DT_FCN:
			ep->type = SYM_AUX_FUN;
			unpack_aux_fun(ORDER(obj->type), buf, &ep->u.aux);
			break;
		case DT_ARY:
			ep->type = SYM_AUX_ARY;
			unpack_aux_ary(ORDER(obj->type), buf, &ep->u.aux);
			break;
		default:
			ep->type = SYM_AUX_SYM;
			unpack_aux_sym(ORDER(obj->type), buf, &ep->u.aux);
		}
	}

	for (++ep; --n > 0; ++ep) {
		ep->type = SYM_AUX_UNK;
		if (fread(ep->u.buf, AUXESZ, 1, fp) != 1)
			return 0;
	}

	return 1;
}

static int
readents(Obj *obj, FILE *fp)
{
	int n;
	long i;
	Entry *ep;
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[SYMESZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_nsyms == 0)
		return 1;

	ep = calloc(hdr->f_nsyms, sizeof(*ep));
	if (!ep)
		return 0;
	coff->ents = ep;

	if (!objpos(obj, fp, hdr->f_symptr))
		return 0;

	for (i = 0; i < hdr->f_nsyms; i++) {
		SYMENT *ent;

		if (fread(buf, SYMESZ, 1, fp) != 1)
			return 0;

		ep = &coff->ents[i];
		ep->type = SYM_ENT;
		ent = &ep->u.sym;
		unpack_ent(ORDER(obj->type), buf, ent);
		if (ent->n_scnum > hdr->f_nscns)
			return 0;

		if (!readauxs(obj, fp, ent, i))
			return 0;

		i += ent->n_numaux;
	}

	return 1;
}

static int
readscns(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	SCNHDR *scn;
	long i;
	unsigned char buf[SCNHSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_nscns == 0)
		return 1;

	scn = calloc(hdr->f_nscns, sizeof(*scn));
	if (!scn)
		return 0;
	coff->scns = scn;

	for (i = 0; i < hdr->f_nscns; i++) {
		if (fread(buf, SCNHSZ, 1, fp) < 0)
			return 0;
		unpack_scn(ORDER(obj->type), buf, &scn[i]);
	}

	return 1;
}

static int
readlines(Obj *obj, FILE *fp)
{
	int i,j;
	LINENO **lines, *lp, *p;
	FILHDR *hdr;
	SCNHDR *scn;
	struct coff32 *coff;
	unsigned char buf[LINESZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_nscns == 0)
		return 1;

	lines = calloc(hdr->f_nscns, sizeof(lp));
	if (!lines)
		return 0;
	coff->lines = lines;

	for (i = 0; i < hdr->f_nscns; i++) {
		scn = &coff->scns[i];
		if (scn->s_nlnno == 0)
			continue;

		lp = calloc(sizeof(*lp), scn->s_nlnno);
		if (!lp)
			return 0;
		lines[i] = lp;

		if (!objpos(obj, fp, scn->s_lnnoptr))
			return 0;

		for (j = 0; j < scn->s_nlnno; j++) {
			if (fread(buf, LINESZ, 1, fp) != 1)
				return 0;
			unpack_line(ORDER(obj->type), buf, lp);
			if (lp->l_lnno != 0			
			&&  lp->l_addr.l_symndx >= hdr->f_nsyms) {
				return 0;
			}
			++lp;
		}
	}

	return 1;
}

static int
readaout(Obj *obj, FILE *fp)
{
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;
	unsigned char buf[AOUTSZ];

	if (hdr->f_opthdr == 0)
		return 1;

	if (fread(buf, AOUTSZ, 1, fp) != 1)
		return 0;

	unpack_aout(ORDER(obj->type), buf, &coff->aout);

	return 1;
}

static int
validate(Obj *obj)
{
	long i, n;
	SYMENT *ent;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	ent = NULL;
	for (i = 0; i < hdr->f_nsyms; i++) {
		SCNHDR *scn;
		AUXENT *aux;
		Entry *ep = &coff->ents[i];

		aux = &ep->u.aux;
		switch (ep->type) {
		case SYM_ENT:
			ent = &ep->u.sym;
			if (ent->n_zeroes != 0 && ent->n_offset > coff->strsiz)
				return -1;
			break;
		case SYM_AUX_FILE:
			if (aux->x_zeroes != 0 && aux->x_offset > coff->strsiz)
				return -1;
			break;
		case SYM_AUX_SCN:
			if (aux->x_scnlen < 0)
				return -1;
			n = ent->n_scnum;
			if (n <= 0)
				return -1;
			scn = &coff->scns[n-1];
			if (scn->s_nrelloc != aux->x_nreloc)
				return -1;
			if (scn->s_nlnno != aux->x_nlinno)
				return -1;
			break;
		case SYM_AUX_FUN:
		case SYM_AUX_SYM:
			if (aux->x_endndx < 0 || aux->x_endndx > hdr->f_nsyms)
				return -1;
		case SYM_AUX_ARY:
			if (aux->x_tagndx < 0 || aux->x_tagndx > hdr->f_nsyms)
				return -1;
			if (aux->x_tvndx < 0 || aux->x_tvndx > hdr->f_nsyms)
				return -1;
			break;
		case SYM_AUX_UNK:
			break;
		default:
			return -1;
		}
	}

	return 0;
}

int
coff32read(Obj *obj, FILE *fp)
{

	if (!readhdr(obj, fp))
		return -1;
	if (!readaout(obj, fp))
		return -1;
	if (!readscns(obj, fp))
		return -1;
	if (!readents(obj, fp))
		return -1;
	if (!readstr(obj, fp))
		return -1;
	if (!readreloc(obj, fp))
		return -1;
	if (!readlines(obj, fp))
		return -1;

	return validate(obj);
}
