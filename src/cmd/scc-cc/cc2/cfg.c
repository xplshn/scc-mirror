/*
 * This file is very tied with nodes.c and the main reason why
 * this is not included directly in node.c is because all this
 * code is not needed in qbe targets.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "cc2.h"

#ifdef NDEBUG
#define PRCFG(msg)
#define PRTREE(msg)
#else
#define PRCFG(msg) (enadebug ? prcfg(msg) : NULL)
#define PRTREE(msg) (enadebug ? prforest(msg) : NULL)
#endif

/* TODO: ignore OCASE, OBSWITCH, ODEFAULT for now */
#define NBLOCKS 10

struct cfg {
	int nr;
	int dirty;
	Block *entryb, *exitb;
	Block *cur;
	Block *blocks;
};

static struct cfg cfg;

#ifndef NDEBUG
#include <stdio.h>

static void
prbb(Block *bb)
{
	if (!bb || bb->printed)
		return;

	assert(bb->id != -1);
	bb->printed = 1;
	if (bb->true) {
		fprintf(stderr,
		        "\t%d -> %d [label=\"true\"]\n",
		        bb->id, bb->true->id);
	}
	if (bb->false) {
		fprintf(stderr,
		        "\t%d -> %d [label=\"false\"]\n",
		        bb->id, bb->false->id);
	}
	prbb(bb->true);
	prbb(bb->false);
}

static void
prcfg(char *msg)
{
	Block *bb;
	Node *np;

	fprintf(stderr, "{digraph %s_%d {\n", msg, curfun->id);

	for (bb = cfg.blocks; bb < &cfg.blocks[cfg.nr]; ++bb) {
		if (bb->id == -1)
			continue;
		bb->printed = 0;

		fprintf(stderr, "\t%d [shape=box;label=\"", bb->id);
		for (np = bb->entryp; np; np = np->next) {
			prtree(np);
			putc('\n', stderr);
			if (np == bb->exitp)
				break;
		}
		fputs("\"]\n", stderr);
	}

	prbb(cfg.entryb);
	fputs("}}\n", stderr);
}
#endif

static Block *
newbb(Node *np)
{
	Block *bb;

	bb = &cfg.blocks[cfg.nr];
	bb->entryp = np;
	bb->exitp = NULL;
	bb->true = bb->false = NULL;
	bb->id = cfg.nr++;
	bb->visited = 0;
	np->bb = bb;

	return bb;
}

static Node *
mkcfg(Range *rp, Node *np)
{
	if ((np->flags & (BBENTRY|BBEXIT)) == 0)
		return np;

	if (np->flags & BBENTRY)
		cfg.cur = np->bb;

	if (np->flags & BBEXIT) {
		cfg.cur->exitp = np;
		cfg.cur->true = np->next->bb;
	}

	switch (np->op) {
	case OBFUN:
		cfg.entryb = cfg.cur;
		break;
	case OEFUN:
		cfg.exitb = cfg.cur;
		break;
	case ORET:
		cfg.cur->true = fbody()->end->bb;
		break;
	case OBRANCH:
		cfg.cur->false = np->next->bb;
	case OJMP:
		cfg.cur->true = np->u.sym->u.stmt->bb;
		break;
	}

	return np;
}

static Node *
mkbb(Range *rp, Node *np)
{
	if (np->flags & BBENTRY)
		newbb(np);
	return np;
}

static void
newentry(Node *np)
{
	if (np->flags & BBENTRY)
		return;

	np->flags |= BBENTRY;
	if (np->prev)
		np->prev->flags |= BBEXIT;
	cfg.nr++;
	DBG("new entry point %d", cfg.nr);
}

static Node *
markbb(Range *rp, Node *np)
{
	switch (np->op) {
	case OBFUN:
		newentry(np);
		break;
	case ORET:
		newentry(rp->end);
		newentry(np->next);
		break;
	case OBRANCH:
	case OJMP:
		newentry(np->u.sym->u.stmt);
		newentry(np->next);
		break;
	}
	return np;
}

static void
visit(Block *bb)
{
	if (!bb || bb->visited)
		return;
	bb->visited = 1;
	visit(bb->true);
	visit(bb->false);
}

static void
trimcfg(void)
{
	Block *bb, *prev, *next;
	Range r;

	visit(cfg.entryb);
	for (bb = cfg.blocks; bb < &cfg.blocks[cfg.nr]; ++bb) {
		if (bb->visited)
			continue;

		r = range(bb->entryp, bb->exitp);
		delrange(&r);
		bb->id = -1;
	}
	PRCFG("trimmed_cfg");
}

static void
buildcfg(void)
{
	int nr;

	apply(fbody(), markbb);
	PRTREE("bb_mark");

	cfg.blocks = xcalloc(cfg.nr, sizeof(Block));
	nr = cfg.nr;
	cfg.nr = 0;
	apply(fbody(), mkbb);
	assert(cfg.nr == nr);

	PRTREE("bb_mk");
	apply(fbody(), mkcfg);
	PRCFG("cfg");
}

static Node *
optlabels(Range *rp, Node *np)
{
	if (np->op == ONOP && np->label->refcnt == 0) {
		cfg.dirty = 1;
		return NULL;
	}
	return np;
}

static Node *
optjmps(Range *rp, Node *np)
{
	Range r;
	Symbol *label;
	Node *p, *stmt, *last;

	switch (np->op) {
	case OBRANCH:
		label = np->u.sym;
		stmt = label->u.stmt;
		goto chain_jumps;
	case OJMP:
		label = np->u.sym;
		stmt = label->u.stmt;

		/* Avoid jump over a set of NOPs */
		r = range(np, rp->end);
		for (p = nextstmt(&r, SETCUR); p; p = nextstmt(&r, SETCUR)) {
			if (p == stmt) {
				cfg.dirty = 1;
				label->refcnt--;
				return NULL;
			}
			if (p->op != ONOP)
				break;
		}

	chain_jumps:
		/* avoid chain of jumps to jumps */
		r = range(stmt, rp->end);
		for (p = stmt; p && p->op == ONOP; p = nextstmt(&r, SETCUR))
			;
		if (p && p != np && p->op == OJMP) {
			cfg.dirty = 1;
			label->refcnt--;
			label = p->u.sym;
			stmt = label->u.stmt;
			np->u.sym = label;
			goto chain_jumps;
		}
	}
}

void
gencfg(void)
{
	PRTREE("before_gencfg");

	cfg.dirty = 1;
	while (cfg.dirty) {
		DBG("new cfg");
		cleancfg();
		buildcfg();
		trimcfg();

		/*
		 * Jump optimizations only can happen after trimming the cfg,
		 * because any change in the jumps make invalid the cfg
		 * automatically
		 */
		apply(fbody(), optjmps);
		apply(fbody(), optlabels);
	}

	PRTREE("after_gencfg");
}

static Node *
cleanbb(Range *rp, Node *np)
{
	np->flags &= ~(BBENTRY | BBEXIT);
	return np;
}

void
cleancfg(void)
{
	free(cfg.blocks);
	memset(&cfg, 0, sizeof(cfg));
	if (curfun)
		apply(fbody(), cleanbb);
}
