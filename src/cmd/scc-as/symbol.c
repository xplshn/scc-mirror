#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <scc/cstd.h>
#include <scc/mach.h>
#include <scc/scc.h>

#include "as.h"

#define HASHSIZ 64
#define NALLOC  10

/*
 * sym must be the first field because we generate
 * a pointer to lsymbol from the symbol
 */
struct lsymbol {
	Symbol sym;
	Section *sec;
	struct lsymbol *next;
	struct lsymbol *hash;
};

/*
 * sec must be the first field because we generate
 * a pointer to lsection from the section
 */
struct lsection {
	Section sec;
	FILE *fp;
	unsigned long long curpc;
	unsigned long long pc;
	struct lsection *next;
};

Section *cursec;
Section *sbss, *sdata, *stext;
Symbol *linesym;
int pass;

static Obj *obj;
static Map *map;
static struct lsection *seclist;
static struct lsymbol *hashtbl[HASHSIZ], *symlast, *symlist;

static Symbol *cursym;
static Alloc *tmpalloc;
static int secindex, symindex;

#ifndef NDEBUG
void
dumpstab(char *msg)
{
	struct lsymbol **bp, *lp;

	fprintf(stderr, "%s\n", msg);
	for (bp = hashtbl; bp < &hashtbl[HASHSIZ]; ++bp) {
		if (*bp == NULL)
			continue;

		fprintf(stderr, "[%d]", (int) (bp - hashtbl));
		for (lp = *bp; lp; lp = lp->hash) {
			fprintf(stderr, " -> %s:%0X:%0llX",
			       lp->sym.name,
			       lp->sym.flags,
			       lp->sym.value);
		}
		putc('\n', stderr);
	}
}
#endif

Symbol *
lookup(char *name)
{
	int r;
	unsigned h;
	Symbol *sym;
	struct lsymbol *lp;
	char *curname, buf[INTIDENTSIZ+1];

	if (*name == '.' && cursym) {
		if (!cursym)
			error("local label '%s' without global label", name);
		curname = cursym->name;
		r = snprintf(buf, sizeof(buf), "%s%s", curname, name);
		if (r < 0 || r >= sizeof(buf))
			error("too long local label '%s%s'", curname, name);
		name = buf;
	}

	h = genhash(name) & HASHSIZ-1;
	for (lp = hashtbl[h]; lp; lp = lp->hash) {
		if (!casecmp(lp->sym.name, name))
			return &lp->sym;
	}

	lp = xmalloc(sizeof(*lp));
	lp->next = NULL;
	lp->hash = hashtbl[h];
	lp->sec = NULL;
	hashtbl[h] = lp;

	if (symlast)
		symlast->next = lp;
	symlast = lp;

	if (!symlist)
		symlist = lp;

	sym = &lp->sym;
	sym->name = xstrdup(name);
	sym->flags = 0;
	sym->size = sym->value = 0;
	sym->section = cursec ? cursec->index : -1;

	return sym;
}

Symbol *
deflabel(char *name)
{
	int local = 0;
	Symbol *sym;
	struct lsection *lsec;
	char label[MAXSYM+1];

	if (*name == '.') {
		int r;

		local = 1;
		if (!cursym) {
			error("local label '%s' without global label", name);
			return NULL;
		}
		r = snprintf(label, sizeof(label),
		             "%s%s",
		             cursym->name, name);
		if (r == sizeof(label)) {
			error("local label '%s' in '%s' produces too long symbol",
			      name, cursym->name);
			return NULL;
		}
		name = label;
	}

	sym = lookup(name);
	if (pass == 1 && (sym->flags & FDEF))
		error("redefinition of label '%s'", name);
	if (cursec->flags & SABS)
		sym->flags |= FABS;

	lsec = (struct lsection *) cursec;
	sym->value = lsec->curpc;
	sym->section = cursec->index;

	if (!local)
		cursym = sym;
	return sym;
}

int
toobig(Node *np, int type)
{
	unsigned long long val = np->sym->value;

	switch (type) {
	case AIMM2:
		return val > 3;
	case AIMM3:
		return val > 7;
	case AIMM5:
		return val > 0x1F;
	case AIMM8:
		return val > 0xFF;
	case AIMM16:
		return val > 0xFFFF;
	case AIMM32:
		return val > 0xFFFFFFFF;
	case AIMM64:
		return 1;
	default:
		abort();
	}
}

unsigned long long
getpc(void)
{
	struct lsection *lsec;

	lsec = (struct lsection *) cursec;
	return lsec->curpc;
}

static void
incpc(int nbytes)
{
	struct lsection *lsec;
	unsigned long long siz;
	TUINT pc, curpc;

	lsec = (struct lsection *) cursec;

	pc = lsec->pc;
	curpc = lsec->curpc;

	lsec->curpc += nbytes;
	lsec->pc += nbytes;

	if (pass == 2)
		return;

	siz = lsec->pc - cursec->base;
	if (siz > cursec->size)
		cursec->size = siz;

	if (pc > lsec->pc ||
	    curpc > lsec->curpc ||
	    lsec->curpc > maxaddr ||
	    lsec->pc > maxaddr) {
		die("as: address overflow in section '%s'");
	}
}

static int
secflags(char *attr)
{
	int c, flags;

	if (!attr)
		return 0;

	for (flags = 0; c = *attr++; ) {
		switch (c) {
		case 'w':
			flags |= SWRITE;
			break;
		case 'r':
			flags |= SREAD;
			break;
		case 'x':
			flags |= SEXEC;
			break;
		case 'c':
			flags |= SALLOC;
			break;
		case 'l':
			flags |= SLOAD;
			break;
		case 'a':
			flags |= SABS;
			break;
		case 'm':
			flags |= SRELOC;
			break;
		default:
			abort();
		}
	}

	return flags;
}

static int
sectype(int flags)
{
	if (flags & SEXEC)
		return 'T';
	if ((flags & (SALLOC|SLOAD|SREAD)) == (SALLOC|SLOAD|SREAD))
		return 'D';
	if ((flags  & (SALLOC|SLOAD|SREAD)) == (SALLOC|SREAD))
		return 'B';
	return '?';
}

static Section *
newsec(Symbol *sym, char *attr)
{
	int idx;
	Section *sec;
	struct lsection *lsec;
	struct lsymbol *lsym;

	if (secindex == INT_MAX) {
		fputs("as: too many sections\n", stderr);
		exit(EXIT_FAILURE);
	}

	lsec = xmalloc(sizeof(*lsec));
	lsec->pc = lsec->curpc = 0;
	lsec->next = seclist;
	lsec->fp = NULL;
	seclist = lsec;

	sec = &lsec->sec;
	sec->name = sym->name;
	sec->base = sec->size = 0;
	sec->flags = 0;
	sec->fill = 0;
	sec->align = 0;
	sec->index = secindex;
	sec->flags |= secflags(attr);
	sec->type = sectype(sec->flags);

	/* sym->flags = ? */
	sym->section = sec->index;
	sym->type = tolower(sec->type);
	sym->index = symindex;
	lsym = (struct lsymbol *) sym;
	lsym->sec = sec;

	if (mapsec(map, sec, NULL, 0) < 0) {
		fprintf(stderr,
		       "as: error allocating section mapping '%s'\n",
		        sym->name);
		exit(EXIT_FAILURE);
	}

	if (!setsec(obj, &secindex, sec)) {
		fprintf(stderr,
		        "as: error adding section '%s' to output\n",
		        sym->name);
		exit(EXIT_FAILURE);
	}

	if (!setsym(obj, &symindex, sym)) {
		fprintf(stderr,
		        "as: error adding section symbol '%s' to output\n",
		        sym->name);
		exit(EXIT_FAILURE);
	}

	secindex++;
	symindex++;

	return sec;
}

Section *
defsec(char *name, char *attr)
{
	struct lsymbol *lsym;
	Section *sec;
	Symbol *sym;

	cursec = NULL;
	sym = lookup(name);
	if (sym->flags & ~FSECT)
		error("invalid section name '%s'", name);

	lsym = (struct lsymbol *) sym;
	sec = lsym->sec;
	if (sec == NULL) {
		sec = newsec(sym, attr);
		lsym->sec = sec;
		sym->section = sec->index;
		sym->flags = FSECT;
	}

	return cursec = sec;
}

void
ibinfmt(void)
{
	int t;

	if ((t = objtype("coff-z80")) < 0) {
		fprintf(stderr,
		        "as: invalid binary format %s\n", "coff32-z80");
		exit(EXIT_FAILURE);
	}

	if ((obj = newobj(t)) < 0) {
		fputs("as: error allocating output\n", stderr);
		exit(EXIT_FAILURE);
	}

	if ((map = newmap(4, 0)) == NULL) {
		perror("as");
		exit(EXIT_FAILURE);
	}

	stext = defsec(".text", "mrxcl");
	sdata = defsec(".data", "mrwcl");
	sbss = defsec(".bss", "rwc");
}

void
cleansecs(void)
{
	int r;
	Section *sec;
	struct lsection *lsec;

	for (lsec = seclist; lsec; lsec = lsec->next) {
		sec = &lsec->sec;
		lsec->curpc = lsec->pc = sec->base;
		if (pass == 1 || (sec->flags & SALLOC) == 0)
			continue;

		lsec->fp = tmpfile();
		r = mapsec(map, sec, lsec->fp, sec->size);

		if (!lsec->fp || r < 0) {
			perror("as: creating section mapping");
			exit(EXIT_FAILURE);
		}
	}
	cursec = stext;
}

void
emit(char *bytes, int n)
{
	struct lsection *lsec = (struct lsection *) cursec;

	if (lsec->fp)
		fwrite(bytes, n, 1, lsec->fp);
	incpc(n);
}

Symbol *
tmpsym(TUINT val)
{
	Symbol *sym;

	if (!tmpalloc)
		tmpalloc = alloc(sizeof(*sym), NALLOC);
	sym = new(tmpalloc);
	sym->value = val;
	sym->section = -1;
	sym->flags = FABS;

	return sym;
}

void
killtmp(void)
{
	if (!tmpalloc)
		return;
	dealloc(tmpalloc);
	tmpalloc = NULL;
}

static int
dumpsec(FILE *src, FILE *dst)
{
	int c;

	if (!src)
		return 0;

	rewind(src);
	while ((c = getc(src)) != EOF)
		putc(c, dst);

	if (ferror(src))
		return -1;

	return 0;
}

void
writecoff(char *fname)
{
	FILE *fp;

	if ((fp = fopen(fname, "wb")) == NULL)
		goto error;

	if (writeobj(obj, map, fp) < 0) {
		fputs("as: corrupted object type\n", stderr);
		goto error;
	}

	if (fclose(fp) == EOF)
		goto error;
	outfile = NULL;
	return;

error:
	fprintf(stderr, "as: %s: error writing output file\n", fname);
	if (errno)
		perror("as");
	exit(EXIT_FAILURE);
}

void
writeout(char *fname)
{
	FILE *fp;
	struct lsection *lp;

	if ((fp = fopen(fname, "wb")) == NULL)
		goto error;


	for (lp = seclist; lp; lp = lp->next) {
		if (dumpsec(lp->fp, fp) < 0)
			goto error;
	}
	fflush(fp);

	if (ferror(fp))
		goto error;

	fclose(fp);
	outfile = NULL;

	return;

error:
	fprintf(stderr, "as: %s: %s\n", fname, strerror(errno));
	exit(EXIT_FAILURE);
}
