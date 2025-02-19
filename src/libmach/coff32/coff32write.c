#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"
#include "fun.h"

struct strtbl {
	char *s;
	long siz;
};

static void
pack_hdr(int order, unsigned char *buf, FILHDR *hdr)
{
	int n;

	n = pack(order,
	         buf,
	         "sslllss",
	         hdr->f_magic,
	         hdr->f_nscns,
	         hdr->f_timdat,
	         hdr->f_symptr,
	         hdr->f_nsyms,
	         hdr->f_opthdr,
	         hdr->f_flags);
	assert(n == FILHSZ);
}

static void
pack_scn(int order, unsigned char *buf, SCNHDR *scn)
{
	int n;

	if (scn->s_zeroes == 0)
		pack(order, buf, "ll", scn->s_zeroes, scn->s_offset);
	else
		memcpy(buf, scn->s_name, 8);

	n = pack(order,
	         buf + 8,
	         "llllllssl",
	         scn->s_paddr,
	         scn->s_vaddr,
	         scn->s_size,
	         scn->s_scnptr,
	         scn->s_relptr,
	         scn->s_lnnoptr,
	         scn->s_nrelloc,
	         scn->s_nlnno,
	         scn->s_flags);
	n += 8;
	assert(n == SCNHSZ);
}

static void
pack_ent(int order, unsigned char *buf, SYMENT *ent)
{
	int n;

	if (ent->n_zeroes == 0)
		pack(order, buf, "ll", ent->n_zeroes, ent->n_offset);
	else
		memcpy(buf, ent->n_name, 8);

	n = pack(order,
	         buf + 8,
	         "lsscc",
		     ent->n_value,
		     ent->n_scnum,
		     ent->n_type,
		     ent->n_sclass,
		     ent->n_numaux);
	n += 8;

	assert(n == SYMESZ);
}

static void
pack_aux_file(int order, unsigned char *buf, AUXENT *aux)
{

	if (aux->x_zeroes == 0) {
		memset(buf, 0, sizeof(AUXESZ));
		pack(order, buf, "ll", aux->x_zeroes, aux->x_offset);
	} else {
		memcpy(buf, aux->x_fname, E_FILNMLEN);
	}
}

static void
pack_aux_scn(int order, unsigned char *buf, AUXENT *aux)
{
	int n;

	n = pack(order,
	         buf,
	         "lsslsc",
	         aux->x_scnlen,
	         aux->x_nreloc,
	         aux->x_nlinno,
	         aux->x_checksum,
	         aux->x_associated,
	         aux->x_comdat);
	assert(n == AUXESZ);
}

static void
pack_aux_fun(int order, unsigned char *buf, AUXENT *aux)
{
	int n;

	n = pack(order,
	         buf,
	         "lllls",
	         aux->x_tagndx,
	         aux->x_fsize,
	         aux->x_lnnoptr,
	         aux->x_endndx,
	         aux->x_tvndx);
	assert(n == AUXESZ);
}

static void
pack_aux_ary(int order, unsigned char *buf, AUXENT *aux)
{
	int n;

	n = pack(order,
	         buf,
	         "lssssss",
	         aux->x_tagndx,
	         aux->x_lnno,
	         aux->x_size,
	         aux->x_dimen[0],
	         aux->x_dimen[1],
	         aux->x_dimen[2],
	         aux->x_dimen[3],
	         aux->x_tvndx);
	assert(n == AUXESZ);
}

static void
pack_aux_sym(int order, unsigned char *buf, AUXENT *aux)
{
	int n;

	n = pack(order,
	         buf,
	         "lsslls",
	         aux->x_tagndx,
	         aux->x_lnno,
	         aux->x_size,
	         aux->x_lnnoptr,
	         aux->x_endndx,
	         aux->x_tvndx);
	assert(n == AUXESZ);
}

static void
pack_aout(int order, unsigned char *buf, AOUTHDR *aout)
{
	int n;

	n = pack(order,
	         buf,
	         "ssllllll",
	         aout->magic,
	         aout->vstamp,
	         aout->tsize,
	         aout->dsize,
	         aout->bsize,
	         aout->entry,
	         aout->text_start,
	         aout->data_start);
	assert(n == AOUTSZ);
}

static void
pack_reloc(int order, unsigned char *buf, RELOC *rel)
{
	int n;

	n = pack(order,
	         buf,
	         "lls",
	         rel->r_vaddr,
	         rel->r_symndx,
	         rel->r_type);
	assert(n == RELSZ);
}

static void
pack_line(int order, unsigned char *buf, LINENO *lp)
{
	int n;

	n = pack(order,
	         buf,
	         "lls",
	         lp->l_addr.l_symndx,
	         lp->l_lnno);
	assert(n == LINESZ);
}

static int
writehdr(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[FILHSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	pack_hdr(ORDER(obj->type), buf, hdr);
	if (fwrite(buf, FILHSZ, 1, fp) != 1)
		return 0;

	return 1;
}

static int
allocstring(struct coff32 *coff, long off, struct strtbl *tbl)
{
	char *s, *name;
	long len, siz;

	siz = tbl->siz;
	name = &coff->strtbl[off];
	len = strlen(name) + 1;
	if (len > siz - LONG_MAX)
		return 0;

	s = realloc(tbl->s, siz + len);
	if (!s)
		return 0;
	memcpy(s + siz, name, len);

	tbl->s = s;
	tbl->siz += len;

	return 1;
}

static int
writescns(Obj *obj, FILE *fp, struct strtbl *tbl)
{
	int i;
	SCNHDR *scn;
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[SCNHSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	for (i = 0; i < hdr->f_nscns; i++) {
		scn = &coff->scns[i];
		if (scn->s_zeroes == 0) {
			if (!allocstring(coff, scn->s_offset, tbl))
				return 0;
		}
		pack_scn(ORDER(obj->type), buf, scn);
		if (fwrite(buf, SCNHSZ, 1, fp) != 1)
			return 0;
	}

	return 1;
}

static int
writeents(Obj *obj, FILE *fp, struct strtbl *tbl)
{
	long i;
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[SYMESZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (!coff->ents)
		return 1;

	for (i = 0; i < hdr->f_nsyms; i++) {
		SYMENT *ent;
		AUXENT *aux;
		Entry *ep = &coff->ents[i];

		aux = &ep->u.aux;
		switch (ep->type) {
		case SYM_ENT:
			ent = &ep->u.sym;
			if (ent->n_zeroes == 0) {
				if (!allocstring(coff, ent->n_offset, tbl))
					return 0;
			}
			pack_ent(ORDER(obj->type), buf, ent);
			break;
		case SYM_AUX_UNK:
			memcpy(buf, ep->u.buf, AUXESZ);
			break;
		case SYM_AUX_SYM:
			pack_aux_file(ORDER(obj->type), buf, aux);
			break;
		case SYM_AUX_FILE:
			if (aux->x_zeroes == 0) {
				if (!allocstring(coff, aux->x_offset, tbl))
					return 0;
			}
			pack_aux_file(ORDER(obj->type), buf, aux);
			break;
		case SYM_AUX_SCN:
			pack_aux_scn(ORDER(obj->type), buf, aux);
			break;
		case SYM_AUX_FUN:
			pack_aux_fun(ORDER(obj->type), buf, aux);
			break;
		case SYM_AUX_ARY:
			pack_aux_ary(ORDER(obj->type), buf, aux);
			break;
		}

		if (fwrite(buf, SYMESZ, 1, fp) != 1)
			return 0;
	}

	return 1;
}

static int
writestr(Obj *obj, FILE *fp, struct strtbl *tbl)
{
	struct coff32 *coff;
	unsigned char buf[4];

	coff = obj->data;

	free(coff->strtbl);
	coff->strtbl = tbl->s;
	coff->strsiz = tbl->siz;
	tbl->s = NULL;
	tbl->siz = 0;

	if ((coff->strsiz & 0xffff) != coff->strsiz)
		return 0;

	pack(ORDER(obj->type), buf, "l", coff->strsiz);
	fwrite(buf, 4, 1, fp);
	fwrite(coff->strtbl, coff->strsiz, 1, fp);

	return 1;
}

static int
writeaout(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[AOUTSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_opthdr == 0)
		return 1;
	pack_aout(ORDER(obj->type), buf, &coff->aout);

	return fread(buf, AOUTSZ, 1, fp) != 1;
}

static int
writereloc(Obj *obj, FILE *fp)
{
	int i, j;
	RELOC *rp;
	SCNHDR *scn;
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[RELSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (!coff->rels)
		return 1;

	for (i = 0; i < hdr->f_nscns; i++) {
		rp = coff->rels[i];
		if (!rp)
			continue;
		scn = &coff->scns[i];

		for (j = 0; j < scn->s_nrelloc; j++) {
			pack_reloc(ORDER(obj->type), buf, &rp[i]);
			if (fwrite(buf, RELSZ, 1, fp) != 1)
				return 0;
		}
	}

	return 1;
}

static int
writelines(Obj *obj, FILE *fp)
{
	int i;
	long j;
	LINENO *lp;
	SCNHDR *scn;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;
	unsigned char buf[LINESZ];

	if (!coff->lines)
		return 1;

        for (i = 0; i < hdr->f_nscns; i++) {
		lp = coff->lines[i];
		if (!lp)
			continue;
		scn = &coff->scns[i];
		for (j = 0; j < scn->s_nlnno; j++) {
			pack_line(ORDER(obj->type), buf, &lp[j]);
			if (fwrite(buf, LINESZ, 1, fp) == 1)
				return 0;
		}
	}

	return 1;
}

static int
writedata(Obj *obj, Map *map, FILE *fp)
{
	long n;
	int id, nsec;
	Mapsec *msec;
	Section *sec;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;
	SCNHDR *scn;

	nsec = hdr->f_nscns;
	for (scn = coff->scns; nsec--; scn++) {
		if ((id = findsec(map, scn->s_name)) < 0)
			continue;
		msec = &map->sec[id];
		sec = &msec->sec;
		if (!msec->fp)
			continue;

		if (copysec(msec, fp) < 0)
			return -1;
	}

	return 1;
}

int
coff32write(Obj *obj, Map *map, FILE *fp)
{
	int id;
	long ptr, n;
	SCNHDR *scn;
	Mapsec *sec;
	struct strtbl tbl;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	ptr = ftell(fp);
	obj->pos = ptr;
	tbl.s = NULL;
	tbl.siz = 0;

	n = hdr->f_nscns;
	ptr += FILHSZ + hdr->f_opthdr + n*SCNHSZ;
	for (scn = coff->scns; n--; scn++) {
		scn->s_scnptr = 0;
		if ((id = findsec(map, scn->s_name)) < 0)
			continue;
		sec = &map->sec[id];
		if (!sec->fp)
			continue;

		scn->s_scnptr = ptr;
		ptr += scn->s_size;
	}
	hdr->f_symptr = (hdr->f_nsyms > 0) ? ptr : 0;

	n = hdr->f_nscns;
	for (scn = coff->scns; n--; scn++) {
		scn->s_relptr = (scn->s_nrelloc > 0) ? ptr : 0;
		ptr += scn->s_nrelloc * RELSZ;
	}

	n = hdr->f_nscns;
	for (scn = coff->scns; n--; scn++) {
		scn->s_lnnoptr = (scn->s_nlnno > 0) ? ptr : 0;
		ptr += scn->s_nlnno * RELSZ;
	}

	/* and now update symbols */

	if (!writehdr(obj, fp))
		goto err;
	if (!writeaout(obj, fp))
		goto err;
	if (!writescns(obj, fp, &tbl))
		goto err;
	if (!writedata(obj, map, fp))
		goto err;
	if (!writereloc(obj, fp))
		goto err;
	if (!writelines(obj, fp))
		goto err;
	if (!writeents(obj, fp, &tbl))
		goto err;
	if (!writestr(obj, fp, &tbl))
		goto err;
	if (ferror(fp))
		goto err;
	return 0;

err:
	free(tbl.s);
	return -1;
}
