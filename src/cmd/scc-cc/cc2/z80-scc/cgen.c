#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "../cc2.h"
#include "arch.h"

#define NLOCALS 16

struct local {
	Symbol *sym;
	struct local *next;
};

static Symbol *retlabel;
static int offpar, offvar;

static Node *reguse[NPAIRS];
static int upper[] = {[DE] = D, [HL] = H, [BC] = B,  [IY] = IYH};
static int lower[] = {[DE] = E, [HL] = L, [BC] = C, [IY] = IYL};
static int pair[] = {
	[A] = A,
	[H] = HL, [L] = HL, [HL] = HL,
	[B] = BC, [C] = BC, [BC] = BC,
	[D] = DE, [E] = DE, [DE] = DE,
	[IYL] = IY, [IYH] = IY, [IY] = IY
};

static Node regs[] = {
	[E] =  {
		.op = OMREG,
		.type.size = 1,
		.u.reg = E
	},
	[D] = {
		.op = OMREG,
		.type.size = 1,
		.u.reg = D
	},
	[H] = {
		.op = OMREG,
		.type.size = 1,
		.u.reg = H
	},
	[L] = {
		.op = OMREG,
		.type.size = 1,
		.u.reg = L
	},
	[C] = {
		.op= OMREG,
		.type.size = 1,
		.u.reg = C
	},
	[B] = {
		.op= OMREG,
		.type.size = 1,
		.u.reg = B
	},
	[A] = {
		.op= OMREG,
		.type.size = 1,
		.u.reg = A
	},
	[IYL] = {
		.op = OMREG,
		.type.size = 1,
		.u.reg = IYL
	},
	[IYH] = {
		.op = OMREG,
		.type.size = 1,
		.u.reg = IYH
	},
	[DE] = {
		.op = OMREG,
		.type.size = 2,
		.u.reg = DE
	},
	[HL] = {
		.op = OMREG,
		.type.size = 2,
		.u.reg = HL
	},
	[BC] = {
		.op = OMREG,
		.type.size = 2,
		.u.reg = BC
	},
	[IX] = {
		.op = OMREG,
		.type.size = 2,
		.u.reg = IX
	},
	[IY] = {
		.op = OMREG,
		.type.size = 2,
		.u.reg = IY
	},
	[SP] = {
		.op = OMREG,
		.type.size = 2,
		.u.reg = SP
	}
};

void
defpar(Symbol *sym)
{
	unsigned align, size;

	if (sym->kind != SREG && sym->kind != SAUTO)
		return;
	align = sym->type.align;
	size = sym->type.size;

	if (offpar == 0)
		offpar = +2;

	sym->u.off = offpar;
	offpar += size;
	offpar += offpar & 1;
}

void
defvar(Symbol *sym)
{
	unsigned long align, size;

	if (sym->kind != SREG && sym->kind != SAUTO)
		return;
	align = sym->type.align;
	size = sym->type.size;

	if (offvar == 0)
		offvar = -2;

	sym->u.off = offvar;
	offvar -= size;
	offvar -= offpar & 1;
}

static Node *
function(void)
{
	Node aux;

	memset(reguse, 0, sizeof(reguse));

	retlabel = newlabel();

	code(ASPUSH, NULL, &regs[IX], NULL);
	code(ASMOV, &regs[IX], &regs[SP], NULL);

	constnode(&aux, offvar + 2, &ptrtype);
	code(ASMOV, &regs[HL], &aux, NULL);
	code(ASADD, &regs[HL], &regs[SP], &regs[HL]);
	code(ASMOV, &regs[SP], &regs[HL], NULL);
}

static void
efunction(void)
{
	setlabel(retlabel);
	code(ASMOV, &regs[SP], &regs[IX], NULL);
	code(ASPOP, &regs[IX], NULL, NULL);
	code(ASRET, NULL, NULL, NULL);
	offpar = offvar = 0;
}

static void
allocreg(Node *np)
{
}

static void
store(Node *np)
{
}

static void
load(Node *np)
{
}

static void
spill(int reg)
{
	Node *np, *r;
	Symbol *sym;
	int p, h, l;

	if ((np = reguse[reg]) == NULL)
		return;
	r = &regs[reg];

	store(r);
	p = pair[reg];
	l = lower[p];
	h = upper[p];

	if (reg >= NREGS)
		reguse[l] = reguse[h] = NULL;
	else
		reguse[reg] = NULL;

	if (!reguse[l] && !reguse[h])
		reguse[p] = NULL;
}

static void
use(int reg, Node *np)
{
	reguse[reg] = np;

	if (reg < NREGS) {
		reguse[pair[reg]] = np;
	} else if (reg >= NREGS && reg < NPAIRS) {
		reguse[lower[reg]] = np;
		reguse[upper[reg]] = np;
	} else {
		abort();
	}
}

static void
load2(Node *np, int dst)
{
	long offl, offh;
	Node low, high;

	switch (np->op) {
	case OAUTO:
		offl = np->u.sym->u.off - 1;
		offh = offl + 1;
		break;
	default:
		abort();
	}

	code(ASLD, &regs[lower[dst]], idxnode(&low, offl), NULL);
	code(ASLD, &regs[upper[dst]], idxnode(&high, offh), NULL);
	use(dst, np);
}

static void
moveto1(Node *np, int dst)
{
	int src;
	Node *pdst = &regs[dst];

	switch (np->op) {
	case OAUTO:
		code(ASLD, pdst, np, NULL);
		break;
	case OMREG:
		src = np->u.reg;
		if (src == dst)
			return;

		code(ASMOV, pdst, &regs[np->u.reg], NULL);
		break;
	default:
		abort();
	}

	use(dst, np);
}

static void
moveto2(Node *np, int dst)
{
	int src;
	Node aux;

	switch (np->op) {
	case OAUTO:
		load2(np, dst);
		break;
	case OMREG:
		src = np->u.reg;
		if (src == dst)
			return;

		if (src == HL && dst == DE || src == DE && dst == HL) {
			code(ASEXHL, &regs[HL], &regs[HL], &regs[DE]);
			use(src, reguse[dst]);
		} else {
			code(ASMOV, &regs[lower[dst]], &regs[lower[src]], NULL);
			code(ASMOV, &regs[upper[dst]], &regs[upper[src]], NULL);
		}
		break;
	default:
		abort();
	}

	use(dst, np);
}

static void
moveto4(Node *np, int dst)
{
	abort();
}

static void
moveto8(Node *np, int dst)
{
	abort();
}

static void
moveto(Node *np, int reg)
{
	switch (np->type.size) {
	case 1:
		moveto1(np, reg);
		break;
	case 2:
		moveto2(np, reg);
		break;
	case 4:
		moveto4(np, reg);
		break;
	case 8:
		moveto4(np, reg);
		break;
	default:
		abort();
	}
}

static int
cmpnode(Node *n1, Node *n2)
{
	if (n1 == n2)
		return 1;
	if (!n1 || !n2)
		return 0;
	if (n1->op != n2->op)
		return 0;

	switch (n1->op) {
	case OAUTO:
		return n1->u.sym == n2->u.sym;
	default:
		return 0;
	}
}

static void
move(Node *np, int reg)
{
	int i;

	switch (np->type.size) {
	case 1:
		for (i = A; i < NREGS; ++i) {
			if (cmpnode(reguse[i], np))
				goto found;
		}
		break;
	case 2:
		for (i = AF; i < NPAIRS; ++i) {
			if (cmpnode(reguse[i], np))
				goto found;
		}
		break;
	case 4:
		abort();
	case 8:
		abort();
	}

	moveto(np, reg);
	return;

found:
	moveto(&regs[i], reg);
}

static int
whatacc(Node *np)
{
	switch (np->type.size) {
	case 1:
		return A;
	case 2:
		return HL;
	case 4:
		return HLDE;
	case 8:
		return EXHLDE;
	default:
		abort();
	}
}

static Node *
acc(Node *np)
{
	int reg = whatacc(np);
	move(np, reg);
	return &regs[reg];
}

static Node *
rhs(Node *np)
{
	switch (np->op) {
	case OAUTO:
		return acc(np);
	case OMEM:
	case OSNEG:
	case OTMP:
	case OCONST:
	case OREG:
	case OMOD:
	case OSHL:
	case OBAND:
	case OBOR:
	case OBXOR:
	case OSHR:
	case ODIV:
	case OLT:
	case OGT:
	case OLE:
	case OGE:
	case OADD:
	case OSUB:
	case OMUL:
	case OEQ:
	case ONE:
	case OCALL:
	case OCALLE:
	case OCAST:
	case OASSIG:
	case OASK:
	case OCOMMA:
	case OPTR:
	case OADDR:
	case OFIELD:
	case OBUILTIN:
	default:
		abort();
	}
}

static Node *
field(Node *np, int islhs)
{
}

static Node *
lhs(Node *np)
{
	switch (np->op) {
	case OMEM:
	case OAUTO:
		return np;
	case OPTR:
		return rhs(np->left);
	case OFIELD:
		return field(np, 1);
	default:
		abort();
	}
}

static void
ret(Node *np)
{
	Node aux;

	if (np->left)
		acc(np->left);
	label2node(&aux, retlabel);
	code(ASJP, NULL, &aux, NULL);
}

static Node *
cgen(Node *np)
{
	Node aux, *next;

	setlabel(np->label);
	switch (np->op) {
	case OBFUN:
		function();
		break;
	case OEFUN:
		efunction();
		break;
	case ONOP:
	case OBLOOP:
	case OELOOP:
		break;
	case OJMP:
		label2node(&aux, np->u.sym);
		code(ASJP, NULL, &aux, NULL);
		break;
	case OBRANCH:
		/* TODO */
		break;
	case ORET:
		ret(np);
		break;
	case OBSWITCH:
		/* TODO */
		break;
	default:
		rhs(np);
		break;
	}
	return np;
}

/*
 * This is strongly influenced by
 * http://plan9.bell-labs.com/sys/doc/compiler.ps (/sys/doc/compiler.ps)
 * calculate addresability as follows
 *     AUTO => 11          value+fp
 *     REG => 13           reg
 *     STATIC => 12        (value)
 *     CONST => 20         $value
 */
Node *
tsethi(Node *np)
{
	Node *l, *r;

	l = np->left;
	r = np->right;
	switch (np->op) {
	case OAUTO:
		np->address = 11;
		break;
	case OREG:
		np->address = 13;
		break;
	case OMEM:
		np->address = 12;
		break;
	case OCONST:
		np->address = 20;
		break;
	default:
		l = sethi(l);
		r = sethi(r);
		break;
	}

	np->left = l;
	np->right = r;

	return np;
}

void
genasm(void)
{
	apply(cgen);
}
