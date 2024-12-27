#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "cc2.h"

#define NNODES   32

struct function {
	Node *begin;
	Node *end;
};

Symbol *curfun;

static Alloc *arena;
static struct function fn;
static Node *curstmt;

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

	save = curstmt;
	curstmt = at;
	addstmt(np, KEEPCUR);

	if (mode == KEEPCUR)
		curstmt = save;
	else
		curstmt = (save == at) ? np : save;

	return np;
}

Node *
addstmt(Node *np, int mode)
{
	Node *next;

	next = NULL;
	if (curstmt) {
		next = curstmt->next;
		if (next)
			next->prev = np;
		curstmt->next = np;
	}
	np->next = next;
	np->prev = curstmt;

	if (!fn.begin)
		fn.begin = np;
	if (!fn.end || !np->next)
		fn.end = np;

	if (mode == SETCUR)
		curstmt = np;

	return np;
}

Node *
delstmt(void)
{
	Node *next, *prev;

	if (!curstmt)
		return NULL;

	next = curstmt->next;
	prev = curstmt->prev;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;

	if (fn.begin == curstmt)
		fn.begin = next;
	if (fn.end == curstmt)
		fn.end = prev;
	deltree(curstmt);

	return curstmt = next;
}

Node *
nextstmt(void)
{
	return curstmt = curstmt->next;
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
	curstmt = NULL;
	curfun = NULL;
	memset(&fn, 0, sizeof(fn));
}

void
apply(Node *(*fun)(Node *))
{
	if (!curfun)
		return;
	curstmt = curfun->u.stmt;
	while (curstmt)
		(*fun)(curstmt) ? nextstmt() : delstmt();
}
