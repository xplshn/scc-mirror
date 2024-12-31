#include <limits.h>
#include <stdlib.h>

#include <scc/scc.h>

#include "cc2.h"

static Swtch *list;

static Node *
swtch_if(Node *idx)
{
	Node *tmpvar, *next, *tmp, *np;
	Symbol *deflabel = NULL;

	tmpvar = tmpnode(&idx->type);
	idx->right = idx->left;
	idx->right = tmpvar;
	idx->op = OASSIG;

	for (np = idx->next; ; np = next) {
		next = np->next;

		switch (np->op) {
		case OESWITCH:
			if (!deflabel)
				deflabel = np->u.sym;
			np->op = OJMP;
			np->u.sym = deflabel;
			return sethi(idx);
		case OCASE:
			np->op = OBRANCH;
			tmp = node(OEQ);
			tmp->type = idx->type;
			tmp->left = np->left;
			tmp->right = tmpvar;
			np->left = tmp;
			break;
		case ODEFAULT:
			deflabel = np->u.sym;
			delstmt(np);
			break;
		default:
			abort();
		}
	}
}

static Node *
swtch_dir(Node *np, int n, TINT min, TINT max)
{
	Node aux, *p, *defnode;
	Node **its, **zero, **ip;
	Symbol *tbl;

	its = xcalloc(n, sizeof(*its));
	zero = its;
	if (min < 0)
		zero -= min;

	for (p = np->next; p->op != OESWITCH; p = p->next) {
		if (p->op == ODEFAULT)
			defnode = p;
		else
			its[p->left->u.i] = p;
		p->type = ptrtype;
		p->op = OLABEL;
	}
	np->next = p->next;

	for (ip = its; ip < &its[n]; ++ip) {
		if (*ip == NULL)
			*ip = defnode;
	}

	tbl = getsym(TMPSYM);
	tbl->kind = SLOCAL;
	tbl->type = ptrtype;
	tbl->type.flags |= INITF;

	defglobal(tbl);
	for (ip = its; ip < &its[n]; ++ip)
		data(*ip);
	endinit();
}

Node *
swtch(Node *np)
{
	int n;
	TINT min, max, range, val;
	Node *p, *def = NULL;

	min = TINT_MAX;
	max = n = 0;
	for (p = np->next; p->op != OESWITCH; p = p->next) {
		if (p->op != ODEFAULT) {
			val = p->left->u.i;
			if (val > max)
				max = val;
			if (val < min)
				min = val;
			++n;
		}
	}

	if (n < 4)
		return swtch_if(np);

	range = max - min + 1;
	if (range == n)
		return swtch_dir(np, range, min, max);

	abort();
}

Swtch *
newswitch(Swtch *swt)
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

	for (p = list; p; p = next) {
		next = p;
		free(p);
	}
}
