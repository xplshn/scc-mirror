#include <stdlib.h>

#include <scc/scc.h>

#include "../cc2.h"
#include "arch.h"

static void
swtch(Node *idx)
{
}

static Node *
rhs(Node *np)
{
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
bool(Node *np, Symbol *true, Symbol *false)
{
	Node *l = np->left, *r = np->right;
	Node ret, ifyes, ifno;
	Symbol *label;

	switch (np->op) {
	case ONEG:
		bool(l, false, true);
		break;
	case OAND:
		label = newlabel();
		bool(l, label, false);
		setlabel(label);
		bool(r, true, false);
		break;
	case OOR:
		label = newlabel();
		bool(l, true, label);
		setlabel(label);
		bool(r, true, false);
		break;
	default:
		label2node(&ifyes, true);
		label2node(&ifno, false);
		code(ASBRANCH, rhs(np), &ifyes, &ifno);
		break;
	}
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
		p = (np->left) ? rhs(np->left) : NULL;
		code(ASRET, NULL, p, NULL);
		break;
	case OBSWITCH:
		swtch(rhs(np->left));
		break;
	default:
		rhs(np);
		break;
	}
	return NULL;
}

void
genasm(void)
{
	apply(cgen);
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
