#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "cc2.h"

#define NNODES   32

Symbol *curfun;

static Alloc *arena;
static Range body;

/*
 * This file defines all the functions required to deal with nodes,
 * and statement ranges. It is usually safe to deal with a subrange
 * of a function because all the functions have an initial beginning
 * statement and an ending statement that are never removed and
 * the begin and end pointer always point to them.
 */
Node *
node(int op)
{
	Node *np;

	if (!arena)
		arena = alloc(sizeof(Node), NNODES);
	np = memset(new(arena), 0, sizeof(*np));
	np->op = op;

	return np;
}

#ifndef NDEBUG
#include <stdio.h>

static void
prnode(Node *np)
{
	if (!np)
		return;
	prnode(np->left);
	prnode(np->right);

	fprintf(stderr, "\t%c%lu", np->op, np->type.size);
}

void
prtree(Node *np)
{
	Block *bb;

	bb = np->bb;

	if (np->flags & BBENTRY)
		putc('>', stderr);
	fprintf(stderr, "(%d)",  bb ? bb->id : 0);
	if (np->flags & BBEXIT)
		putc('>', stderr);

	prnode(np);
	putc('\n', stderr);
}

void
prforest(char *msg)
{
	Node *np;

	fprintf(stderr, "tree %s {\n", msg);
	for (np = body.begin; np; np = np->next)
		prtree(np);
	fputs("}\n", stderr);
}
#endif

/*
 * Insert a node after `at' and if mode is SETCUR
 * update the current statement if `at' was the
 * current statement
 */
Node *
insstmt(Range *rp, Node *np, Node *at, int mode)
{
	Node *save;

	save = rp->cur;
	rp->cur = at;
	addstmt(rp, np, KEEPCUR);

	if (mode == KEEPCUR)
		rp->cur = save;
	else
		rp->cur = (save == at) ? np : save;

	return np;
}

Node *
addstmt(Range *rp, Node *np, int mode)
{
	Node *next;

	next = NULL;
	if (rp->cur) {
		next = rp->cur->next;
		if (next)
			next->prev = np;
		rp->cur->next = np;
	}
	np->next = next;
	np->prev = rp->cur;

	if (!rp->begin)
		rp->begin = np;
	if (!rp->end || !np->next)
		rp->end = np;

	if (mode == SETCUR)
		rp->cur = np;

	return np;
}

Node *
delstmt(Range *rp)
{
	Node *next, *prev;

	next = rp->cur->next;
	prev = rp->cur->prev;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;

	if (rp->begin == rp->cur)
		rp->begin = next;
	if (rp->end == rp->cur)
		rp->end = prev;
	deltree(rp->cur);

	if (rp->cur == rp->end)
		return rp->cur = NULL;
	return rp->cur = next;
}

Node *
nextstmt(Range *rp, int mode)
{
	Node *next;

	next = (rp->cur == rp->end) ? NULL : rp->cur->next;
	if (mode == SETCUR)
		rp->cur = next;
	return next;
}

void
delnode(Node *np)
{
	delete(arena, np);
}

void
deltree(Node *np)
{
	if (!np)
		return;
	deltree(np->left);
	deltree(np->right);
	delnode(np);
}

void
cleannodes(void)
{
	if (arena) {
		dealloc(arena);
		arena = NULL;
	}
	curfun = NULL;
}

void
newfun(Symbol *sym)
{
	curfun = sym;
	body = range(NULL, NULL);
	curfun->u.body = &body;
}

void
delrange(Range *rp)
{
	Node *lprev, *rnext, *next, *np;

	lprev = rp->begin->prev;
	rnext = rp->end->next;

	if (lprev)
		lprev->next = rnext;
	if (rnext)
		rnext->prev = lprev;

	for (np = rp->begin; np != rnext; np = next) {
		next = np->next;
		deltree(np);
	}

	rp->begin = rp->end = rp->cur = NULL;
}

Range
range(Node *begin, Node *end)
{
	Range r;

	r.cur = r.begin = begin;
	r.end = end;
	return r;
}

void
apply(Range *rp, Node *(*fun)(Range *, Node *))
{
	Node *np;

	rp->cur = rp->begin;
	while (rp->cur) {
		np = (*fun)(rp, rp->cur);
		if (!np) {
			delstmt(rp);
		} else {
			rp->cur = np;
			nextstmt(rp, SETCUR);
		}
	}
}
