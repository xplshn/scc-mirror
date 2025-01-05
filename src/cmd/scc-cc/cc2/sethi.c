#include <scc/scc.h>

#include "cc2.h"

static Node *
assignnode(Type *tp, Node *lhs, Node *rhs)
{
	Node *np;

	np = node(OASSIG);
	np->type = *tp;
	np->left = lhs;
	np->right = rhs;
	return sethi(np);
}

static Node *
branchnode(Node *cond, Symbol *sym)
{
	Node *np;
	int op = cond ? OBRANCH : OJMP;

	np = node(op);
	np->op = op;
	np->left = sethi(cond);
	np->u.sym = sym;
	sym->refcnt++;

	return np;
}

static Node *
bool(Node *cond, Symbol *true, Symbol *false)
{
	Symbol *label;
	Node *p, *l = cond->left, *r = cond->right;

	switch (cond->op) {
	case ONEG:
		l = bool(l, false, true);
		break;
	case OAND:
		label = newlabel();
		l = bool(l, label, false);
		prestmt(labelstmt(NULL, label));
		r = bool(r, true, false);
		break;
	case OOR:
		label = newlabel();
		l = bool(l, true, label);
		prestmt(labelstmt(NULL, label));
		r = bool(r, true, false);
		break;
	default:
		prestmt(branchnode(cond, true));
		prestmt(branchnode(NULL, false));
		return NULL;
	}

	cond->left = l;
	cond->right = r;
	return cond;
}

Node *
logicexpr(Node *np)
{
	Node *tmpvar, *p, *zero, *one;
	Type *tp = &np->type;
	Symbol *true, *false, *phi, *tmpsym;

	true = newlabel();
	false = newlabel();
	phi = newlabel();

	bool(np, true, false);

	tmpvar = tmpnode(tp, NULL);
	tmpsym = tmpvar->u.sym;
	zero = constnode(NULL, 0, tp);
	one = constnode(NULL, 1, tp);

	p = assignnode(tp, tmpnode(tp, tmpsym), zero);
	prestmt(labelstmt(p, false));

	prestmt(branchnode(NULL, phi));

	p = assignnode(tp, tmpnode(tp, tmpsym), one);
	prestmt(labelstmt(p, true));

	prestmt(labelstmt(NULL, phi));
	delstmt(np);

	return sethi(tmpvar);
}

Node *
sethi(Node *np)
{
	int op;
	Node *next, *l, *r;

	if (!np)
		return np;

	np->complex = 0;
	np->address = 0;

	switch (np->op) {
	case OBSWITCH:
		np = swtch(np);
		break;
	case OBRANCH:
		next = np->next;
		if (!next->label)
			labelstmt(next, NULL);

		op = np->left->op;
		if (op != ONEG && op != OOR && op != OAND) {
			sethi(np->left);
			return np;
		}

		bool(np->left, np->u.sym, next->label);
		np->u.sym->refcnt--;
		return NULL;
	case ONEG:
	case OAND:
	case OOR:
		np = logicexpr(np);
		break;
	default:
		np = tsethi(np);
		break;
	}

	l = np->left;
	r = np->right;

	if (np->address > 10)
		return np;
	if (l)
		np->complex = l->complex;
	if (r) {
		int d = np->complex - r->complex;

		if (d == 0)
			++np->complex;
		else if (d < 0)
			np->complex = r->complex;
	}
	if (np->complex == 0)
		++np->complex;

	return np;
}

void
genaddr(void)
{
	apply(sethi);
	PRTREE("after sethi");
}
