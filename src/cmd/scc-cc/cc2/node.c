#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "cc2.h"

#define NNODES   32

struct function {
	Node *begin;
	Node *end;
	Node *cur;
};

Symbol *curfun;

static Alloc *arena;
static struct function fn;

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
	for (np = fn.begin; np; np = np->next)
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

	save = fn.cur;
	fn.cur = at;
	addstmt(np, KEEPCUR);

	if (mode == KEEPCUR)
		fn.cur = save;
	else
		fn.cur = (save == at) ? np : save;

	return np;
}

Node *
addstmt(Node *np, int mode)
{
	Node *next;

	next = NULL;
	if (fn.cur) {
		next = fn.cur->next;
		if (next)
			next->prev = np;
		fn.cur->next = np;
	}
	np->next = next;
	np->prev = fn.cur;

	if (!fn.begin)
		fn.begin = np;
	if (!fn.end || !np->next)
		fn.end = np;

	if (mode == SETCUR)
		fn.cur = np;

	return np;
}

Node *
delstmt(void)
{
	Node *next, *prev;

	if (!fn.cur)
		return NULL;

	next = fn.cur->next;
	prev = fn.cur->prev;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;

	if (fn.begin == fn.cur)
		fn.begin = next;
	if (fn.end == fn.cur)
		fn.end = prev;
	deltree(fn.cur);

	return fn.cur = next;
}

Node *
nextstmt(void)
{
	return fn.cur = fn.cur->next;
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
	fn.cur = NULL;
	curfun = NULL;
	memset(&fn, 0, sizeof(fn));
}

void
apply(Node *(*fun)(Node *))
{
	if (!curfun)
		return;
	fn.cur = fn.begin;
	while (fn.cur)
		(*fun)(fn.cur) ? nextstmt() : delstmt();
}
