#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "cc2.h"

#define NNODES   32

Symbol *curfun;

static Alloc *arena;
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

	switch (np->op) {
	case OBRANCH:
	case OJMP:
		fprintf(stderr,
		        "\t%c -> L%u",
		        np->op, np->u.sym->numid);
		break;
	case OCONST:
		fprintf(stderr,
		        "\t%c%lu{%llu}",
		        np->op, np->type.size, np->u.i);
		break;
	case OAUTO:
	case OREG:
	case OMEM:
	case OLABEL:
	case OTMP:
		fprintf(stderr,
		        "\t%c%lu[%u]",
		        np->op, np->type.size, np->u.sym->numid);
		break;
	default:
		fprintf(stderr,
		        "\t%c%lu",
		        np->op, np->type.size);
		break;
	}
	fprintf(stderr,"(%d,%d)", np->complex, np->address);
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
	putc('\t', stderr);
	if (np->label)
		fprintf(stderr, "L%u:", np->label->numid);

	prnode(np);
	putc('\n', stderr);
}

void
prforest(char *msg)
{
	Node *np;

	fprintf(stderr, "tree %s {\n", msg);
	for (np = curfun->u.stmt; np; np = np->next)
		prtree(np);
	fputs("}\n", stderr);
}
#endif

Node *
insstmt(Node *np, Node *at)
{
	Node *next;

	next = at->next;
	if (next)
		next->prev = np;
	at->next = np;

	np->next = next;
	np->prev = at;

	return np;
}

Node *
unlinkstmt(Node *np)
{
	Node *next, *prev;

	next = np->next;
	prev = np->prev;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;
	np->next = np->prev = NULL;

	return np;
}

Node *
prestmt(Node *np)
{
	return insstmt(np, curstmt->prev);
}

Node *
addstmt(Node *np)
{
	insstmt(np, curstmt);
	return curstmt = np;
}

Node *
delstmt(Node *np)
{
	Node *next;

	next = np->next;
	deltree(unlinkstmt(np));
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
newfun(Symbol *sym, Node *np)
{
	curfun = sym;
	curstmt = curfun->u.stmt = np;
}

void
delrange(Node *begin, Node *end)
{
	Node *lprev, *rnext, *next, *np;

	lprev = begin->prev;
	rnext = end->next;

	if (lprev)
		lprev->next = rnext;
	if (rnext)
		rnext->prev = lprev;

	for (np = begin; np != rnext; np = next) {
		next = np->next;
		deltree(np);
	}
}

void
apply(Node *(*fun)(Node *))
{
	Node *np, *ocurstmt;

	for (curstmt = curfun->u.stmt; curstmt; curstmt = np) {
		ocurstmt = curstmt;
		np = (*fun)(curstmt);
		np = (np) ? np->next : delstmt(ocurstmt);
	}
}
