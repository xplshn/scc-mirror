#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/cstd.h>
#include <scc/scc.h>

#include "../cc2.h"
#include "arch.h"

#define ADDR_LEN (INTIDENTSIZ+64)

enum segment {
	CODESEG,
	DATASEG,
	BSSSEG,
	NOSEG
};

static void src(void), dst(void), bin(void), lit(void), jmp(void);

static struct opdata {
	void (*fun)(void);
	char *txt;
} optbl[] = {
	[ASPUSH] = {.fun = src, .txt = "push"},
	[ASPOP] = {.fun = dst, .txt = "pop"},

	[ASRET] = {.fun = lit, .txt = "ret"},
	[ASJP] = {.fun = jmp, .txt = "jp"},
	[ASJP] = {.fun = jmp, .txt = "jr"},
	[ASBRANCH] = {0},

	[ASMOV] = {.fun = bin, .txt = "ld"},
	[ASEXHL] = {.fun = bin, .txt = "ex"},

	[ASADD] = {.fun = bin, .txt = "add"},
	[ASLD] = {.fun = bin, .txt = "ld"},
};

static int curseg = NOSEG;
static unsigned long offpar, offvar;

static void
segment(int seg)
{
	static char *txt[] = {
		[CODESEG] = "\t.text\n",
		[DATASEG] = "\t.data\n",
		[BSSSEG] = "\t.bss\n",
	};

	if (seg == curseg)
		return;
	fputs(txt[seg], stdout);
	curseg = seg;
}

static char *
symname(Symbol *sym)
{
	static char name[ADDR_LEN];

	if (sym->name) {
		switch (sym->kind) {
		case SGLOB:
		case SEXTRN:
			snprintf(name, sizeof(name), "_%s", sym->name);
			return name;
		case SPRIV:
			return sym->name;
		}
	}

	sprintf(name, ".%d", sym->numid);

	return name;
}

static void
label(Symbol *sym)
{
	int seg;
	char *name = symname(sym);

	if (sym->type.flags & FUNF)
		seg = CODESEG;
	else if (sym->type.flags & INITF)
		seg = DATASEG;
	else
		seg = BSSSEG;
	segment(seg);

	switch (sym->kind) {
	case SEXTRN:
		printf("\t.extrn\t%s\n", name);
		return;
	case SGLOB:
		printf("\t.globl\t%s\n", name);
		break;
	}

	printf("%s:\n", name);
}

static void
emitconst(Node *np)
{
	switch (np->type.size) {
	case 1:
		printf("%d", (int) np->u.i & 0xFF);
		break;
	case 2:
		printf("%d", (int) np->u.i & 0xFFFF);
		break;
	case 4:
		printf("%ld", (long) np->u.i & 0xFFFFFFFF);
		break;
	default:
		abort();
	}
}

static void
emittree(Node *np)
{
	if (!np)
		return;

	switch (np->op) {
	case OSTRING:
		pprint(np->u.s);
		free(np->u.s);
		np->u.s = NULL;
		break;
	case OCONST:
		emitconst(np);
		break;
	case OADDR:
		emittree(np->left);
		break;
	case OLABEL:
	case OMEM:
		fputs(symname(np->u.sym), stdout);
		break;
	default:
		emittree(np->left);
		printf(" %c ", np->op);
		emittree(np->right);
		break;
	}
}

static void
size2asm(Type *tp)
{
	char *s;

	/*
	 * In z80 we can ignore the alignment
	 */
	if (tp->flags & STRF) {
		s = "\tDB\t";
	} else {
		switch (tp->size) {
		case 1:
			s = "\tDB\t";
			break;
		case 2:
			s = "\tDW\t";
			break;
		case 4:
			s = "\tDD\t";
			break;
		default:
			s = "\tDS\t%lu,";
			break;
		}
	}
	printf(s, tp->size);
}

void
deftype(Type *tp)
{
}

void
defglobal(Symbol *sym)
{
	label(sym);
	if (sym->kind == SEXTRN || (sym->type.flags & INITF))
		return;
	size2asm(&sym->type);
	puts("0");
}

void
data(Node *np)
{
	size2asm(&np->type);
	emittree(np);
	putchar('\n');
}

void
writeout(void)
{
	label(curfun);

	for (pc = prog; pc; pc = pc->next) {
		if (pc->label)
			printf("%s:\n", symname(pc->label));
		if (pc->op == ASLABEL)
			continue;
		(*optbl[pc->op].fun)();
	}
}

void
endinit(void)
{
}

static char *
addr2txt(Addr *a)
{
	static char *regnames[] = {
		[A] = "a",
		[B] = "b",
		[C] = "c",
		[D] = "d",
		[E] = "e",
		[H] = "h",
		[L] = "l",
		[IYL] = "iyl",
		[IYH] = "iyh",
		[AF] = "af",
		[HL] = "hl",
		[DE] = "de",
		[BC] = "bc",
		[IY] = "iy",
		[SP] = "sp",
		[IX] = "ix",
	};
	static char addr[INTIDENTSIZ+1];

	switch (a->kind) {
	case SREG:
		return regnames[a->u.reg];
	case SINDEX:
		sprintf(addr,"%ld(ix)", a->u.off);
		return addr;
	case SLABEL:
	case STMP:
	case SGLOB:
	case SEXTRN:
	case SPRIV:
	case SLOCAL:
		return symname(a->u.sym);
	case SCONST:
		sprintf(addr, "%lld", a->u.i);
		return addr;
	default:
		abort();
	}
}

static void
lit(void)
{
	struct opdata *p = &optbl[pc->op];

	printf("\t%s\n", p->txt);
}

static void
bin(void)
{
	struct opdata *p = &optbl[pc->op];
	char to[ADDR_LEN], from[ADDR_LEN];

	strcpy(from, addr2txt(&pc->from1));
	strcpy(to, addr2txt(&pc->to));
	printf("\t%s\t%s,%s\n", p->txt, from, to);
}

static void
src(void)
{
	struct opdata *p = &optbl[pc->op];
	char from[ADDR_LEN];

	strcpy(from, addr2txt(&pc->from1));
	printf("\t%s\t%s\n", p->txt, from);
}

static void
dst(void)
{
	struct opdata *p = &optbl[pc->op];
	char to[ADDR_LEN];

	strcpy(to, addr2txt(&pc->to));
	printf("\t%s\t%s\n", p->txt, to);
}

static void
jmp(void)
{
	struct opdata *p = &optbl[pc->op];

	printf("\t%s\t%s\n", p->txt, addr2txt(&pc->from1));
}
