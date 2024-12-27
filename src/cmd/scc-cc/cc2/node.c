#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "cc2.h"

#define NNODES   32

Symbol *curfun;

static Alloc *arena;
static Range body;

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

	if (!curfun)
		return;

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
insstmt(Node *np, Node *at, int mode)
{
	Node *save;

	save = body.cur;
	body.cur = at;
	addstmt(np, KEEPCUR);

	if (mode == KEEPCUR)
		body.cur = save;
	else
		body.cur = (save == at) ? np : save;

	return np;
}

Node *
addstmt(Node *np, int mode)
{
	Node *next;

	next = NULL;
	if (body.cur) {
		next = body.cur->next;
		if (next)
			next->prev = np;
		body.cur->next = np;
	}
	np->next = next;
	np->prev = body.cur;

	if (!body.begin)
		body.begin = np;
	if (!body.end || !np->next)
		body.end = np;

	if (mode == SETCUR)
		body.cur = np;

	return np;
}

Node *
delstmt(void)
{
	Node *next, *prev;

	if (!body.cur)
		return NULL;

	next = body.cur->next;
	prev = body.cur->prev;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;

	if (body.begin == body.cur)
		body.begin = next;
	if (body.end == body.cur)
		body.end = prev;
	deltree(body.cur);

	return body.cur = next;
}

Node *
nextstmt(void)
{
	return body.cur = body.cur->next;
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
	body.cur = NULL;
	curfun = NULL;
	memset(&body, 0, sizeof(body));
}

void
newfun(Symbol *sym)
{
	curfun = sym;
	curfun->u.body = memset(&body, 0, sizeof(body));
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
apply(Node *(*fun)(Node *))
{
	if (!curfun)
		return;
	body.cur = body.begin;
	while (body.cur)
		(*fun)(body.cur) ? nextstmt() : delstmt();
}
