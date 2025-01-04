#include <scc/scc.h>

#include "cc2.h"

static Node *
bool(Node *np, Symbol *true, Symbol *false)
{
	Symbol *label;
	Node *p, *l = np->left, *r = np->right;

	switch (np->op) {
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
		p = node(OBRANCH);
		p->left = sethi(np);
		p->u.sym = true;
		true->refcnt++;
		prestmt(p);

		p = node(OJMP);
		p->u.sym = false;
		false->refcnt++;
		prestmt(p);
		return NULL;
	}

	np->left = l;
	np->right = r;
	return np;
}

Node *
logicexpr(Node *np)
{
	Node *tmpvar, *p;
	Type *tp = &np->type;
	Symbol *true, *false, *phi;

	true = newlabel();
	false = newlabel();
	phi = newlabel();

	bool(np, true, false);

	tmpvar = tmpnode(tp, NULL);

	p = node(OASSIG);
	p->type = *tp;
	p->left = tmpnode(tp, tmpvar->u.sym);
	p->right = constnode(NULL, 0, tp);
	prestmt(labelstmt(sethi(p), false));

	p = node(OJMP);
	p->u.sym = phi;
	phi->refcnt++;
	prestmt(p);

	p = node(OASSIG);
	p->type = *tp;
	p->left = tmpnode(tp, tmpvar->u.sym);
	p->right = constnode(NULL, 1, tp);
	prestmt(labelstmt(sethi(p), true));

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
