#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/cstd.h>
#include <scc/scc.h>

#include "cc2.h"

#define STACKSIZ     50

Type funetype = {
	.flags = FUNF | ELLIPS
};

Type funtype = {
	.flags = FUNF
};

union tokenop {
	void *arg;
	unsigned op;
};

Node *laststmt;
static Swtch swtbl[NR_BLOCK], *swp = swtbl;
static Symbol *lastfun;

typedef void parsefun(char *, union tokenop);
static parsefun type, symbol, getname, unary, binary, ternary, call,
                constant, composed, binit, einit,
                jump, oreturn, loop, assign,
                ocase, bswitch, eswitch, builtin;

typedef void evalfun(void);
static evalfun vardecl, beginfun, endfun, endpars, stmt,
               array, aggregate, flddecl, labeldcl;

static struct decoc {
	void (*eval)(void);
	void (*parse)(char *token, union tokenop);
	union tokenop u;
} optbl[] = {      /*  eval     parse           args */
	['A']   = {  vardecl,  symbol, .u.op  =  SAUTO<<8 | OAUTO},
	['R']   = {  vardecl,  symbol, .u.op  =   SREG<<8 |  OREG},
	['G']   = {  vardecl,  symbol, .u.op  =  SGLOB<<8 |  OMEM},
	['X']   = {  vardecl,  symbol, .u.op  = SEXTRN<<8 |  OMEM},
	['Y']   = {  vardecl,  symbol, .u.op  =  SPRIV<<8 |  OMEM},
	['T']   = {  vardecl,  symbol, .u.op  = SLOCAL<<8 |  OMEM},
	['M']   = {  flddecl,  symbol, .u.op  =  SMEMB<<8 |  OMEM},
	['L']   = { labeldcl,  symbol, .u.op  = SLABEL<<8 | OLABEL},

	['C']   = {     NULL,    type, .u.arg =    &int8type},
	['I']   = {     NULL,    type, .u.arg =   &int16type},
	['W']   = {     NULL,    type, .u.arg =   &int32type},
	['Q']   = {     NULL,    type, .u.arg =   &int64type},
	['K']   = {     NULL,    type, .u.arg =   &uint8type},
	['N']   = {     NULL,    type, .u.arg =  &uint16type},
	['Z']   = {     NULL,    type, .u.arg =  &uint32type},
	['O']   = {     NULL,    type, .u.arg =  &uint64type},
	['J']   = {     NULL,    type, .u.arg = &float32type},
	['D']   = {     NULL,    type, .u.arg = &float64type},
	['H']   = {     NULL,    type, .u.arg = &float80type},
	['0']   = {     NULL,    type, .u.arg =    &voidtype},
	['B']   = {     NULL,    type, .u.arg =    &booltype},
	['P']   = {     NULL,    type, .u.arg =     &ptrtype},
	['E']   = {     NULL,    type, .u.arg =    &funetype},
	['1']	= {     NULL,    type, .u.arg =    &arg_type},

	['F']   = {     NULL,    type, .u.arg =     &funtype},
	['V']   = {    array,composed,                     0},
	['U']   = {aggregate,composed,                     0},
	['S']   = {aggregate,composed,                     0},

	['"']   = {     NULL, getname,                     0},
	['{']   = { beginfun,    NULL,                     0},
	['}']   = {   endfun,    NULL,                     0},
	['(']   = {     NULL,   binit,                     0},
	[')']   = {     NULL,   einit,                     0},
	['\\']  = {  endpars,    NULL,                     0},
	['\t']  = {     stmt,    NULL,                     0},

	['~']   = {     NULL,   unary, .u.op =          OCPL},
	['_']   = {     NULL,   unary, .u.op =         OSNEG},
	['\'']  = {     NULL,   unary, .u.op =         OADDR},
	['@']   = {     NULL,   unary, .u.op =          OPTR},
	['g']   = {     NULL,   unary, .u.op =         OCAST},
	['p']   = {     NULL,   unary, .u.op =          OPAR},
	['n']   = {     NULL,   unary, .u.op =          ONEG},

	['a']   = {     NULL,  binary, .u.op =          OAND},
	['o']   = {     NULL,  binary, .u.op =           OOR},
	['.']   = {     NULL,  binary, .u.op =        OFIELD},
	['+']   = {     NULL,  binary, .u.op =          OADD},
	['-']   = {     NULL,  binary, .u.op =          OSUB},
	['*']   = {     NULL,  binary, .u.op =          OMUL},
	['%']   = {     NULL,  binary, .u.op =          OMOD},
	['/']   = {     NULL,  binary, .u.op =          ODIV},
	['l']   = {     NULL,  binary, .u.op =          OSHL},
	['r']   = {     NULL,  binary, .u.op =          OSHR},
	['<']   = {     NULL,  binary, .u.op =           OLT},
	['>']   = {     NULL,  binary, .u.op =           OGT},
	['[']   = {     NULL,  binary, .u.op =           OLE},
	[']']   = {     NULL,  binary, .u.op =           OGE},
	['=']   = {     NULL,  binary, .u.op =           OEQ},
	['!']   = {     NULL,  binary, .u.op =           ONE},
	['&']   = {     NULL,  binary, .u.op =         OBAND},
	['|']   = {     NULL,  binary, .u.op =          OBOR},
	['^']   = {     NULL,  binary, .u.op =         OBXOR},
	[',']   = {     NULL,  binary, .u.op =        OCOMMA},
	['m']   = {     NULL,  builtin,.u.op =      OBUILTIN},

	[':']   = {     NULL,  assign, .u.op =        OASSIG},
	['?']   = {     NULL, ternary, .u.op =          OASK},
	['c']   = {     NULL,    call, .u.op =         OCALL},
	['z']   = {     NULL,    call, .u.op =        OCALLE},

	['#']   = {     NULL,constant, .u.op =        OCONST},

	['j']   = {     NULL,    jump, .u.op =          OJMP},
	['y']   = {     NULL,    jump, .u.op =       OBRANCH},
	['h']   = {     NULL, oreturn, .u.op =          ORET},
	['i']   = {     NULL,    NULL, .u.op =          OINC},
	['d']   = {     NULL,    NULL, .u.op =          ODEC},

	['b']   = {     NULL,    loop, .u.op =        OBLOOP},
	['e']   = {     NULL,    loop, .u.op =        OELOOP},

	['v']   = {     NULL,   ocase, .u.op =         OCASE},
	['f']   = {     NULL,   ocase, .u.op =      ODEFAULT},
	['t']   = {     NULL, eswitch, .u.op =      OESWITCH},
	['s']   = {     NULL, bswitch, .u.op =      OBSWITCH},
};

static int sclass, inpars, ininit, beginf, endf, lineno;
static void *stack[STACKSIZ], **sp = stack;

static Node *
push(void *elem)
{
	if (sp == &stack[STACKSIZ])
		error(ESTACKO);
	return *sp++ = elem;
}

static void *
pop(void)
{
	if (sp == stack)
		error(ESTACKU);
	return *--sp;
}

static int
empty(void)
{
	return sp == stack;
}

static void
type(char *token, union tokenop u)
{
	push(u.arg);
}

static void
composed(char *token, union tokenop u)
{
	Symbol *sym;

	sym = getsym(atoi(token+1));
	sym->type.id = sym->id;
	push(&sym->type);
}

static void
getname(char *t, union tokenop u)
{
	push((*++t) ? xstrdup(t) : NULL);
}

static void
symbol(char *token, union tokenop u)
{
	Node *np = node(u.op & 0xFF);
	Symbol *sym = getsym(atoi(token+1));

	sclass = u.op >> 8;
	np->u.sym = sym;
	np->type = sym->type;
	push(np);
}

static Type *
gettype(char *token)
{
	struct decoc *dp;

	dp = &optbl[*token];
	if (!dp->parse)
		error(ESYNTAX);
	(*dp->parse)(token, dp->u);
	return pop();
}

static void
constant(char *token, union tokenop u)
{
	static char letters[] = "0123456789ABCDEF";
	Node *np;
	TUINT v;
	unsigned c;

	++token;
	if (*token == '"') {
		++token;
		np = node(OSTRING);
		np->type.flags = STRF;
		np->type.size = strlen(token);
		np->type.align = int8type.align;
		np->u.s = xstrdup(token);
	} else {
		np = node(OCONST);
		np->type = *gettype(token++);
		for (v = 0; c = *token++; v += c) {
			v <<= 4;
			c = strchr(letters, c) - letters;
		}
		np->u.i = v;
	}
	push(np);
}

static void
assign(char *token, union tokenop u)
{
	int subop;
	Node *np = node(u.op);

	switch (subop = *++token) {
	case '+':
	case '-':
	case '*':
	case '%':
	case '/':
	case 'l':
	case 'r':
	case '&':
	case '|':
	case '^':
	case 'i':
	case 'd':
		++token;
		subop = optbl[subop].u.op;
		break;
	default:
		subop = 0;
		break;
	}

	np->u.subop = subop;
	np->type = *gettype(token);
	np->right = pop();
	np->left = pop();
	push(np);
}

static void
ternary(char *token, union tokenop u)
{
	Node *ask = node(OASK), *colon = node(OCOLON);
	Type *tp = gettype(token+1);

	colon->right = pop();
	colon->left = pop();

	ask->type = *tp;
	ask->left = pop();
	ask->right = colon;
	push(ask);
}

static void
eval(char *tok)
{
	struct decoc *dp;

	do {
		dp = &optbl[*tok];
		if (!dp->parse)
			break;
		(*dp->parse)(tok, dp->u);
	} while (tok = strtok(NULL, "\t\n"));
}

static int
nextline(void)
{
	static char line[LINESIZ];
	size_t len;
	int c;
	void (*fun)(void);

repeat:
	++lineno;
	if (!fgets(line, sizeof(line), stdin))
		return 0;
	if ((len = strlen(line)) == 0 || line[0] == '\n')
		goto repeat;
	if (line[len-1] != '\n')
		error(len < sizeof(line)-1 ? ELNBLNE : ELNLINE);
	line[len-1] = '\0';

	c = *line;
	eval(strtok(line, "\t\n"));
	if ((fun = *optbl[c].eval) != NULL)
		(*fun)();
	if (sp != stack)
		error(ESTACKA);
	return 1;
}

static void
oreturn(char *token, union tokenop u)
{
	Node *np = node(u.op);

	if (token = strtok(NULL, "\t\n"))
		eval(token);
	if (!empty())
		np->left = pop();
	push(np);
}

static void
addcase(Node *np)
{
	TINT val;
	Node **bp;
	Swtch *cur;

	if (swp == swtbl)
		error(EWTACKU);

	cur = swp - 1;
	if (np->op == ODEFAULT) {
		cur->defnode = np;
	} else {
		cur->nr++;
		bp = cur->cases;
		bp = xrealloc(bp, cur->nr * sizeof(Node *));
		bp[cur->nr-1] = np;
		cur->cases = bp;

		val = np->left->u.i;
		if (val < cur->min)
			cur->min = val;
		if (val > cur->max)
			cur->max = val;
	}
}

static void
bswitch(char *token, union tokenop u)
{
	Swtch *cur;
	Node *np = node(u.op);

	if (swp == &swtbl[NR_BLOCK])
		error(EWTACKO);
	cur = swp++;
	cur->nr = 0;
	cur->min = TINT_MAX;
	cur->max = TINT_MIN;
	cur->defnode = NULL;
	cur->cases = NULL;
	eval(strtok(NULL, "\t\n"));
	np->left = pop();

	push(cur->bswtch = np);
}

static int
cmpcase(const void *p1, const void *p2)
{
	TINT d;
	Node *np1, *np2;

	np1 = *(Node **) p1;
	np2 = *(Node **) p2;
	d = np1->left->u.i - np2->left->u.i;
	if (d < 0)
		return -1;
	if (d > 0)
		return 1;
	return 0;
}

static void
eswitch(char *token, union tokenop u)
{
	Node *p;
	Swtch *cur;

	if (swp == swtbl)
		error(EWTACKU);
	jump(token, u);

	cur = --swp;
	cur->eswtch = pop();
	cur->bswtch->u.swtch = newswtch(cur);
	qsort(cur->cases, cur->nr, sizeof(Node *), cmpcase);
}

static void
ocase(char *token, union tokenop u)
{
	jump(token, u);
	addcase(pop());
}

static void
jump(char *token, union tokenop u)
{
	Symbol *label;
	Node *aux, *np = node(u.op);

	eval(strtok(NULL, "\t\n"));

	if (u.op == OBRANCH || u.op == OCASE)
		np->left = pop();
	aux = pop();
	label = aux->u.sym;
	label->refcnt++;
	np->u.sym = label;
	delnode(aux);
	push(np);
}

static void
loop(char *token, union tokenop u)
{
	push(node(u.op));
}

static void
unary(char *token, union tokenop u)
{
	Node *np = node(u.op);

	np->type = *gettype(token+1);
	np->left = pop();
	np->right = NULL;
	push(np);
}

static void
call(char *token, union tokenop u)
{
	Node *np, *par, *fun = node(u.op);

	for (par = NULL;; par = np) {
		np = pop();
		if (np->op != OPAR)
			break;
		np->right = par;
	}

	fun->type = *gettype(token+1);
	fun->left = np;
	fun->right = par;
	push(fun);
}

static void
builtin(char *token, union tokenop u)
{
	Node *np = node(u.op);
	char *name;
	unsigned subop, nchilds;

	np->type = *gettype(token+1);
	name = pop();

	if (!strcmp("__builtin_va_arg", name)) {
		nchilds = 1;
		subop = BVA_ARG;
	} else if (!strcmp("__builtin_va_start", name)) {
		nchilds = 2;
		subop = BVA_START;
	} else if (!strcmp("__builtin_va_end", name)) {
		nchilds = 1;
		subop = BVA_END;
	} else if (!strcmp("__builtin_va_copy", name)) {
		nchilds = 2;
		subop = BVA_COPY;
	} else {
		error(EBBUILT);
	}

	np->u.subop = subop;
	np->right = (nchilds == 2) ? pop() : NULL;
	np->left = (nchilds != 0) ? pop() : NULL;

	free(name);
	push(np);
}

static void
binary(char *token, union tokenop u)
{
	Node *np = node(u.op);

	np->type = *gettype(token+1);
	np->right = pop();
	np->left = pop();
	push(np);
}

static void
binit(char *token, union tokenop u)
{
	ininit = 1;
}

static void
einit(char *token, union tokenop u)
{
	ininit = 0;
	endinit();
}

static void
endpars(void)
{
	if (!inpars)
		error(ESYNTAX);
	inpars = 0;
}

static void
aggregate(void)
{
	Node *align, *size;
	char *name;
	Type *tp;
	Symbol *sym;

	align = pop();
	size = pop();
	name = pop();
	tp = pop();

	tp->size = size->u.i;
	tp->align = align->u.i;
	tp->flags = AGGRF;
	/*
	 * type is the first field of Symbol so we can obtain the
	 * address of the symbol from the address of the type.
	 * We have to do this because composed returns the pointer
	 * to the type, but in this function we also need the
	 * symbol to store the name.
	 */
	sym = (Symbol *) tp;
	sym->name = name;
	deftype(tp);

	delnode(align);
	delnode(size);
}

static void
array(void)
{
	Type *tp, *base;
	Node *size;

	size = pop();
	base = pop();
	tp = pop();
	tp->flags = ARRF;

	if (size->u.i > LONG_MAX/base->size)
		error(EOVERFL);

	tp->size = size->u.i * base->size;
	tp->align = base->align;

	delnode(size);
}

static void
decl(Symbol *sym)
{
	Type *tp = &sym->type;

	if (tp->flags & FUNF) {
		lastfun = sym;
	} else {
		switch (sym->kind) {
		case SEXTRN:
		case SGLOB:
		case SPRIV:
		case SLOCAL:
			defglobal(sym);
			break;
		case SAUTO:
		case SREG:
			if (!beginf)
				error(ESYNTAX);
			((inpars) ? defpar : defvar)(sym);
			break;
		default:
			abort();
		}
	}
}

static void
vardecl(void)
{
	Type *tp, *rp;
	Node *np;
	Symbol *sym;
	char *name;

	name = pop();
	tp = pop();
	if (tp->flags & FUNF)
		rp = pop();
	np = pop();

	sym = np->u.sym;
	/*
	 * We have to free sym->name because in tentative declarations
	 * we can have multiple declarations of the same symbol, and in
	 * this case our parser will allocate twice the memory
	 */
	free(sym->name);
	sym->name = name;
	sym->type = *tp;
	if (tp->flags & FUNF)
		sym->rtype = *rp;
	sym->kind = sclass;

	if (ininit)
		sym->type.flags |= INITF;
	decl(sym);
	delnode(np);
}

static void
flddecl(void)
{
	Node *off, *np;
	char *name;
	Type *tp;
	Symbol *sym;

	off = pop();
	name = pop();
	tp = pop();
	np = pop();

	sym = np->u.sym;
	sym->u.off = off->u.i;
	sym->name = name;
	sym->type = *tp;

	delnode(np);
	delnode(off);
}

static void
labeldcl(void)
{
	Node *np;
	Symbol *sym;

	np = pop();
	np->op = ONOP;
	sym = np->u.sym;
	sym->kind = SLABEL;
	labelstmt(np, sym);
	addstmt(np);
}

static void
stmt(void)
{
	Node *np;

	if (empty())
		return;
	np = pop();
	if (ininit) {
		data(np);
		deltree(np);
		return;
	}
	addstmt(np);
}

static void
beginfun(void)
{
	newfun(lastfun, node(OBFUN));
	beginf = inpars = 1;
	pushctx();
}

static void
endfun(void)
{
	endf = 1;
	laststmt = addstmt(node(OEFUN));
}

void
parse(void)
{
	/* clean from previous function */
	cleanswtch();
	cleancfg();
	cleannodes();
	popctx();

	inpars = ininit = beginf = endf = 0;

	while (!endf && nextline())
		;

	if (ferror(stdin))
		error(EFERROR, strerror(errno));

	if (beginf && !endf)
		error(EBAFFUN);

	PRTREE("after parsing");
}
