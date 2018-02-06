static char sccsid[] = "@(#) ./cc2/arch/z80/cgen.c";

#include "arch.h"
#include "../../../inc/scc.h"
#include "../../cc2.h"

static void
swtch(Node *idx)
{
}

static Node *
lhs(Node *np, Node *ret)
{
}

static Node *
rhs(Node *np, Node *ret)
{
}

static void
bool(Node *np, Symbol *true, Symbol *false)
{
}

Node *
cgen(Node *np)
{
	Node aux, *p, *next;

	setlabel(np->label);
	switch (np->op) {
	case OJMP:
		label2node(&aux, np->u.sym);
		code(ASJMP, NULL, &aux, NULL);
		break;
	case OBRANCH:
		next = np->next;
		if (!next->label)
			next->label = newlabel();
		bool(np->left, np->u.sym, next->label);
		break;
	case ORET:
		p = np->left;
		if (p)
			p = rhs(np->left, &aux);
		code(ASRET, NULL, p, NULL);
		break;
	case OBSWITCH:
		swtch(rhs(np->left, &aux));
		break;
	default:
		rhs(np, &aux);
		break;
	}
	return NULL;
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
sethi(Node *np)
{
	Node *lp, *rp;

	if (!np)
		return np;

	np->complex = 0;
	np->address = 0;
	lp = np->left;
	rp = np->right;
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
		sethi(lp);
		sethi(rp);
		break;
	}

	if (np->address > 10)
		return np;
	if (lp)
		np->complex = lp->complex;
	if (rp) {
		int d = np->complex - rp->complex;

		if (d == 0)
			++np->complex;
		else if (d < 0)
			np->complex = rp->complex;
	}
	if (np->complex == 0)
		++np->complex;
	return np;
}
