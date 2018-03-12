
static char sccsid[] = "@(#) ./nm/coff.c";

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/coff32/filehdr.h"
#include "../inc/coff32/scnhdr.h"
#include "../inc/coff32/syms.h"
#include "../inc/scc.h"
#include "nm.h"

static int (*unpack)(unsigned char *, char *, ...);
static long stringtbl;
static SCNHDR *sections;
static struct symbol *syms;
static size_t nsect, nsyms;

static char
typeof(SYMENT *ent)
{
	SCNHDR *sec;
	int c;
	long flags;
	char **bp;

	switch (ent->n_scnum) {
	case N_DEBUG:
		c = '?';
		break;
	case N_ABS:
		c = 'A';
		break;
	case N_UNDEF:
		c = 'U';
		break;
	default:
		if (ent->n_scnum > nsect)
			die("nm:incorrect section index");
		sec = &sections[ent->n_scnum-1];
		flags = sec->s_flags;
		if (flags & STYP_TEXT)
			c = 't';
		else if (flags & STYP_DATA)
			c = 'd';
		else if (flags & STYP_BSS)
			c = 'b';
		else
			c = '?';

		if (ent->n_sclass == C_EXT)
			c = toupper(c);

		break;
	}
	return c;
}

static char *
getsname(FILE *fp, SYMENT *ent)
{
	int c;
	size_t len;
	char *s;
	fpos_t pos;

	if (ent->n_zeroes != 0) {
		for (len = 0; len < E_SYMNMLEN && ent->n_name[len]; ++len)
			;
		s = xmalloc(len+1);
		s[len] = '\0';
		return memcpy(s, ent->n_name, len);
	}

	
	fgetpos(fp, &pos);
	fseek(fp, stringtbl, SEEK_SET);
	fseek(fp, ent->n_offset, SEEK_CUR);

	if (ferror(fp))
		goto error;

	s = xmalloc(1);
	for (len = 0; (c = getc(fp)) != '\0'; len++) {
		if (c == EOF)
			goto error;
		s = xrealloc(s, len+1);
		s[len] = c;
	}
	s[len] = '\0';
	fsetpos(fp, &pos);
	return s;

error:
	fprintf(stderr,
	        "nm::%s\n",
	        (ferror(fp)) ? strerror(errno) : "broken string table");
	exit(1);
}

static void
getfsym(unsigned char *buff, SYMENT *ent)
{
	int n;

	n = (*unpack)(buff,
	              "'8lsscc",
	              &ent->n_name,
	              &ent->n_value,
	              &ent->n_scnum,
	              &ent->n_type,
	              &ent->n_sclass,
	              &ent->n_numaux);
	assert(n == SYMESZ);
}

static void
getsymbol(FILE *fp, unsigned char *buff, SYMENT *ent, struct symbol *sym)
{
	char *nam;

	getfsym(buff, ent);
	nam = ent->n_name;
	if (nam[0] == 0 && nam[1] == 0 && nam[2] == 0 && nam[3] == 0) {
		long zero, offset;

		(*unpack)(nam, "ll", &zero, &offset);
		ent->n_zeroes = zero;
		ent->n_offset = offset;
	}
	sym->name = getsname(fp, ent);
	sym->type = typeof(ent);
	sym->value = ent->n_value;
}

static void
getsyms(char *fname, char *member, FILE *fp, FILHDR *hdr)
{
	size_t n, i;
	unsigned aux;
	unsigned char buff[SYMESZ];
	SYMENT ent;

	if (hdr->f_nsyms > SIZE_MAX)
		die("nm:%s:Too many symbols\n", member);

	n = hdr->f_nsyms;
	syms = xcalloc(sizeof(*syms), n);

	if (fseek(fp, hdr->f_symptr, SEEK_SET) == EOF)
		die("nm:%s:%s", member, strerror(errno));

	aux = nsyms = 0;
	for (i = 0; i < n; i++) {
		if (fread(buff, SYMESZ, 1, fp) != 1)
			break;
		if (aux > 0) {
			aux--;
			continue;
		}
		getsymbol(fp, buff, &ent, &syms[nsyms++]);
		aux = ent.n_numaux;
	}
	if (n != i) {
		die("nm:%s:%s",
		    member,
		    (ferror(fp)) ? strerror(errno) : "EOF before reading symbols");
	}
}

static void
getfsec(unsigned char *buff, SCNHDR *sec)
{
	int n;

	n = (*unpack)(buff,
	              "'8llllllssl",
	              sec->s_name,
	              &sec->s_paddr,
	              &sec->s_vaddr,
	              &sec->s_size,
	              &sec->s_scnptr,
	              &sec->s_relptr,
	              &sec->s_lnnoptr,
	              &sec->s_nrelloc,
	              &sec->s_nlnno,
	              &sec->s_flags);
	assert(n == SCNHSZ);
}

static void
getsects(char *fname, char *member, FILE *fp, FILHDR *hdr)
{
	size_t i;
	char buff[SCNHSZ];

	nsect = hdr->f_nscns;
	if (nsect == 0)
		return;

	if (nsect > SIZE_MAX)
		die("nm:%s:Too many sections\n", member);

	if (fseek(fp, FILHSZ + hdr->f_opthdr, SEEK_SET) == EOF)
		die("nm:%s:%s", member, strerror(errno));

	sections = xcalloc(sizeof(*sections), nsect);
	for (i = 0; i < nsect; i++) {
		if (fread(buff, SCNHSZ, 1, fp) != 1)
			break;
		getfsec(buff, &sections[i]);
	}
	if (i != nsect) {
		fprintf(stderr,
			"nm:%s:%s\n",
		        member,
			(ferror(fp)) ? strerror(errno) : "EOF before reading sections");
		exit(1);
	}
}

static void
getfhdr(unsigned char *buff, FILHDR *hdr)
{
	int n;

	n = (*unpack)(buff,
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
nm(char *fname, char *member, FILE *fp)
{
	unsigned char buff[FILHSZ];
	FILHDR hdr;
	unsigned magic;

	if (fread(buff, FILHSZ, 1, fp) != 1)
		return;

	magic = buff[0] | buff[1] << 8;

	switch (magic) {
	case COFF_Z80MAGIC:
		unpack = lunpack;
		break;
	default:
		abort();
	}

	getfhdr(buff, &hdr);
	if ((hdr.f_flags & F_SYMS) != 0 || hdr.f_nsyms == 0) {
		fprintf(stderr, "nm: %s: no symbols\n", member);
		return;
	}

	stringtbl = hdr.f_symptr + hdr.f_nsyms* SYMESZ;

	getsects(fname, member, fp, &hdr);
	getsyms(fname, member, fp, &hdr);
	printsyms(fname, member, syms, nsyms);

	free(sections);
	free(syms);
}

static int
probe(FILE *fp)
{
	int c;
	int c1, c2;
	fpos_t pos;
	static unsigned short magic;

	fgetpos(fp, &pos);
	c1 = getc(fp);
	c2 = getc(fp);
	fsetpos(fp, &pos);

	if (c1 == EOF || c2 == EOF)
		return 0;
	magic = c1 | c2 << 8;

	switch (magic) {
	case COFF_Z80MAGIC:
		return 1;
	default:
		return 0;
	}
}

struct objfile coff32 = {
	.probe = probe,
	.nm = nm,
};
