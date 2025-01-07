#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "cc2.h"

struct cfg {
	int nr;
	Block *entryb, *exitb;
	Block *cur;
	Block *blocks;
};

static struct cfg cfg;
static int modjmp;

static Node *
jtarget(Node *np)
{
	return np->u.sym->u.stmt;
}

static Block *
jtargetbb(Node *np)
{
	return jtarget(np)->bb;
}

#ifndef NDEBUG
#include <stdio.h>

static void
prbb(Block *bb)
{
	Swtch *swt;
	Block *casebb;
	Node **cases, **bp;

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

	swt = bb->swtch;
	if (!swt)
		return;
	cases = swt->cases;

	for (bp = cases; bp < &cases[swt->nr]; ++bp) {
		casebb = jtargetbb(*bp);
		fprintf(stderr,
			"\t%d -> %d [label=\"case %lld\"]\n",
			bb->id,
			casebb->id,
			(*bp)->left->u.i);
		prbb(casebb);
	}

	casebb = jtargetbb(swtchdefault(swt));
	fprintf(stderr,
		"\t%d -> %d [label=\"default\"]\n",
		bb->id,
		casebb->id);
	prbb(casebb);
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
	bb->swtch = NULL;
	bb->true = bb->false = NULL;
	bb->id = cfg.nr++;
	bb->visited = 0;
	cfg.cur = bb;

	return bb;
}

static Node *
mkcfg(Node *np)
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
		cfg.cur->true = laststmt->bb;
		break;
	case OBSWITCH:
		cfg.cur->swtch = np->u.swtch;
		cfg.cur->true = NULL;
		break;
	case OBRANCH:
		cfg.cur->false = np->next->bb;
	case OJMP:
		cfg.cur->true = jtargetbb(np);
		break;
	}

	return np;
}

static Node *
mkbb(Node *np)
{
	if (np->flags & BBENTRY)
		newbb(np);
	np->bb = cfg.cur;

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
markbb(Node *np)
{
	Swtch *swt;
	Node **cases, **bp;

	switch (np->op) {
	case OBFUN:
		newentry(np);
		break;
	case ORET:
		newentry(laststmt);
		newentry(np->next);
		break;
	case OBSWITCH:
		np->flags |= BBEXIT;
		swt = np->u.swtch;
		cases = swt->cases;
		for (bp = cases; bp < &cases[swt->nr]; ++bp)
			newentry(jtarget((*bp)));
		newentry(jtarget(swtchdefault(swt)));
		break;
	case OBRANCH:
	case OJMP:
		newentry(jtarget(np));
		newentry(np->next);
		break;
	}
	return np;
}

static void
visit(Block *bb)
{
	Swtch *swt;
	Node **cases, **bp, *p;

	if (!bb || bb->visited)
		return;
	bb->visited = 1;
	visit(bb->true);
	visit(bb->false);

	swt = bb->swtch;
	if (!swt)
		return;
	cases = swt->cases;

	for (bp = swt->cases; bp < &cases[swt->nr]; ++bp)
		visit(jtargetbb(*bp));
	visit(jtargetbb(swtchdefault(swt)));
}

static void
trimcfg(void)
{
	Block *bb, *prev, *next;

	visit(cfg.entryb);
	for (bb = cfg.blocks; bb < &cfg.blocks[cfg.nr]; ++bb) {
		if (bb->visited)
			continue;
		delrange(bb->entryp, bb->exitp);
		bb->id = -1;
	}
	PRCFG("trimmed_cfg");
}

static void
buildcfg(void)
{
	int nr;

	apply(markbb);
	PRTREE("bb_mark");

	cfg.blocks = xcalloc(cfg.nr, sizeof(Block));
	nr = cfg.nr;
	cfg.nr = 0;
	apply(mkbb);
	assert(cfg.nr == nr);

	PRTREE("bb_mk");
	apply(mkcfg);
	PRCFG("cfg");
}

static int
negop(int op)
{
	switch (op) {
	case OEQ:  return ONE;
	case ONE:  return OEQ;
	case OLT:  return OGE;
	case OGE:  return OLT;
	case OLE:  return OGT;
	case OGT:  return OLE;
	default:   abort();
	}
	return op;
}

static Node *
skipnop(Node *np, Node *target)
{
	for ( ; np->op == ONOP && np != target; np = np->next)
		;
	return np;
}

static Node *
optjmps_helper(Node *np)
{
	Symbol *label;
	Node *p, *stmt, *next;

	switch (np->op) {
	case ONOP:
		if (np->label->refcnt == 0) {
			modjmp = 1;
			return NULL;
		}
		break;
	case OBRANCH:
	branch:
		label = np->u.sym;
		stmt = label->u.stmt;
		next = np->next;

		/* avoid branch over jump */
		if (next->op == OJMP && next->next->label == label &&
		    (!next->label || next->label->refcnt == 0)) {
			Node *left = np->left;
			np->u.sym = next->u.sym;
			label->refcnt--;
			left->op = negop(left->op);
			delstmt(next);
			modjmp = 1;
			goto branch;
		}
		goto chain_jumps;
	case OJMP:
		label = np->u.sym;
		stmt = label->u.stmt;

		/* avoid jump over a set of NOPs */
		p = skipnop(np->next, stmt);
		if (p == stmt) {
			label->refcnt--;
			modjmp = 1;
			return NULL;
		}

	chain_jumps:
		/* avoid chain of jumps to jumps */
		p = skipnop(stmt, NULL);
		if (p != np && p->op == OJMP) {
			label->refcnt--;
			label = p->u.sym;
			stmt = label->u.stmt;
			np->u.sym = label;
			modjmp = 1;
			goto chain_jumps;
		}
	}

	return np;
}

static void
optjmps(void)
{
	do {
		modjmp = 0;
		apply(optjmps_helper);
		PRTREE("after_modjmp");
	} while (modjmp == 1);
}

void
gencfg(void)
{
	optjmps();
	DBG("new cfg");
	buildcfg();
	trimcfg();

	PRTREE("after_gencfg");
}

void
cleancfg(void)
{
	free(cfg.blocks);
	memset(&cfg, 0, sizeof(cfg));
}
