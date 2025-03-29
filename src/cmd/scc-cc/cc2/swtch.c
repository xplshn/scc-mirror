#include <limits.h>
#include <stdlib.h>

#include <scc/scc.h>

#include "cc2.h"

static Swtch *list;

static Node *
swtch_if(Node *np)
{
	Type *tp;
	Swtch *swt;
	Node **cases, **bp, *tmpvar, *p;

	swt = np->u.swtch;
	tp = &np->left->type;

	tmpvar = tmpnode(tp, NULL);
	np->type = *tp;
	np->right = np->left;
	np->left = tmpvar;
	np->op = OASSIG;
	np->u.subop = 0;

	cases = swt->cases;
	for (bp = cases; bp < &cases[swt->nr]; ++bp) {
		Node *eq, *tmp = tmpnode(tp, tmpvar->u.sym);

		p = *bp;
		eq = node(OEQ);
		eq->type = *tp;
		eq->left = p->left;
		eq->right = tmp;
		*bp = NULL;

		p->left = eq;
		p->op = OBRANCH;
		addstmt(p);
	}
	p = swtchdefault(swt);
	p->op = OJMP;
	addstmt(p);

	free(cases);
	swt->cases = NULL;

	return sethi(np);
}

static Node *
swtch_dir(Node *np, TINT min, TINT max)
{
	int i;
	TINT cur, nval;
	Swtch *swt;
	Symbol *tbl;
	Node *p, *def, **cases;

	tbl = getsym(TMPSYM);
	tbl->kind = SLOCAL;
	tbl->type = ptrtype;
	tbl->type.flags |= INITF;
	defglobal(tbl);

	swt = np->u.swtch;
	cases = swt->cases;

	def = swtchdefault(swt);
	def->type = ptrtype;
	def->op = OLABEL;

	i = 0;
	p = NULL;
	for (cur = min; cur <= max; ++cur) {
		if (!p && i < swt->nr) {
			p = cases[i++];
			p->type = ptrtype;
			p->op = OLABEL;
			nval = p->left->u.i;
		}
		if (p && nval == cur) {
			data(p);
			p = NULL;
		} else {
			data(def);
		}
	}
	endinit();

	return np;
}

Node *
swtch(Node *np)
{
	Swtch *swt;
	int n;
	TINT min, max, range;

	swt = np->u.swtch;
	min = swt->min;
	max = swt->max;
	range = max - min + 1;
	n = swt->nr;

	if (n < mach.swtchif)
		return swtch_if(np);
	return swtch_dir(np, min, max);
}

Node *
swtchdefault(Swtch *swt)
{
	Node *np;

	np = swt->defnode;
	if (!np)
		np = swt->eswtch;
	return np;
}

Swtch *
newswtch(Swtch *swt)
{
	Swtch *p = xmalloc(sizeof(*p));

	*p = *swt;
	p->next = list;
	return list = p;
}

void
cleanswtch(void)
{
	Swtch *p, *next;
	Node **bp, **cases;

	for (p = list; p; p = next) {
		next = p->next;
		cases = p->cases;
		if (cases) {
			for (bp = cases; bp < &cases[p->nr]; ++p)
				deltree(*bp);
			free(cases);
		}
		free(p);
	}
}
