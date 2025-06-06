#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <scc/scc.h>
#include "cc1.h"

static void emitbin(int, void *),
            emitcast(int, void *),
            emitsym(int, void *),
            emitexp(int, void *),
            emitsymid(int, void *),
            emittext(int, void *),
            emitfun(int, void *),
            emitdcl(int, void *),
            emitinit(int, void *),
            emittype(int, void *),
            emitbuilt(int, void *);

char *optxt[] = {
	[OADD] = "+",
	[OSUB] = "-",
	[OMUL] = "*",
	[OINC] = ":i",
	[ODEC] =  ":d",
	[OPTR] = "@",
	[OMOD] = "%",
	[ODIV] = "/",
	[OSHL] = "l",
	[OSHR]  = "r",
	[OLT] = "<",
	[OGT] = ">",
	[OGE] = "]",
	[OLE] =  "[",
	[OEQ] = "=",
	[ONE] = "!",
	[OBAND] = "&",
	[OBXOR]  = "^",
	[OBOR] = "|",
	[OASSIGN] = ":",
	[OA_MUL] = ":*",
	[OA_DIV] = ":/",
	[OA_MOD] = ":%",
	[OA_ADD] = ":+",
	[OA_SUB] = ":-",
	[OA_SHL] = ":l",
	[OA_SHR] = ":r",
	[OA_AND] = ":&",
	[OA_XOR] = ":^",
	[OA_OR] = ":|",
	[OADDR] = "'",
	[OSNEG] = "_",
	[ONEG] = "n",
	[OCPL] = "~",
	[OAND] = "a",
	[OOR] = "o",
	[OASK] = "?",
	[OCOMMA] = ",",
	[OLABEL] = "L%d\n",
	[ODEFAULT] = "\tf\tL%d\n",
	[OBSWITCH] = "\ts",
	[OESWITCH] = "\tt\tL%d\n",
	[OCASE] = "\tv\tL%d",
	[OJUMP] = "\tj\tL%d\n",
	[OBRANCH] = "\ty\tL%d",
	[OEFUN] = "}\n",
	[OELOOP] = "\te\n",
	[OBLOOP] = "\tb\n",
	[ORET] = "\th",
	[OPAR] = "p",
	[OCALL] = "c",
	[OCALLE] = "z",
	[OFIELD] = "."
};

void (*opcode[])(int, void *) = {
	[OADD] = emitbin,
	[OSUB] = emitbin,
	[OMUL] = emitbin,
	[OINC] = emitbin,
	[ODEC] =  emitbin,
	[OPTR] = emitbin,
	[OMOD] = emitbin,
	[ODIV] = emitbin,
	[OSHL] = emitbin,
	[OSHR]  = emitbin,
	[OLT] = emitbin,
	[OGT] = emitbin,
	[OGE] = emitbin,
	[OLE] =  emitbin,
	[OEQ] = emitbin,
	[ONE] = emitbin,
	[OBAND] = emitbin,
	[OBXOR]  = emitbin,
	[OBOR] = emitbin,
	[OASSIGN] = emitbin,
	[OA_MUL] = emitbin,
	[OA_DIV] = emitbin,
	[OA_MOD] = emitbin,
	[OA_ADD] = emitbin,
	[OA_SUB] = emitbin,
	[OA_SHL] = emitbin,
	[OA_SHR] = emitbin,
	[OA_AND] = emitbin,
	[OA_XOR] = emitbin,
	[OA_OR] = emitbin,
	[OADDR] = emitbin,
	[OSNEG] = emitbin,
	[ONEG] = emitbin,
	[OCPL] = emitbin,
	[OAND] = emitbin,
	[OOR] = emitbin,
	[OCOMMA] = emitbin,
	[OCAST] = emitcast,
	[OSYM] = emitsym,
	[OASK] = emitbin,
	[OCOLON] = emitbin,
	[OFIELD]= emitbin,
	[OEXPR] = emitexp,
	[OLABEL] = emitsymid,
	[ODEFAULT] = emitsymid,
	[OCASE] = emitsymid,
	[OJUMP] = emitsymid,
	[OBRANCH] = emitsymid,
	[OEFUN] = emittext,
	[OELOOP] = emittext,
	[OBLOOP] = emittext,
	[OFUN] = emitfun,
	[ORET] = emittext,
	[ODECL] = emitdcl,
	[OBSWITCH] = emittext,
	[OESWITCH] = emitsymid,
	[OPAR] = emitbin,
	[OCALL] = emitbin,
	[OCALLE] = emitbin,
	[OINIT] = emitinit,
	[OBUILTIN] = emitbuilt,
	[OTYP] = emittype,
};

static FILE *outfp;

void
icode(void)
{
	outfp = stdout;
}

void
freetree(Node *np)
{
	if (!np)
		return;
	freetree(np->left);
	freetree(np->right);
	free(np);
}

static void
emitnode(Node *np)
{
	if (np)
		(*opcode[np->op])(np->op, np);
}

Node *
prtree(char *s, Node *np)
{
	FILE *tmp = outfp;

	outfp = stderr;
	fprintf(outfp, "DBG prtree %s", s);
	emitnode(np);
	putc('\n', outfp);
	outfp = tmp;

	return np;
}

void
emit(int op, void *arg)
{
	extern int failure;

	if (failure || onlycpp || onlyheader)
		return;
	(*opcode[op])(op, arg);
}

static void
emitvar(Symbol *sym)
{
	int c;
	short flags = sym->flags;

	if (flags & SLOCAL)
		c = 'T';
	else if (flags & SPRIVATE)
		c =  'Y';
	else if (flags & SGLOBAL)
		c = 'G';
	else if (flags & SREGISTER)
		c = 'R';
	else if (flags & SFIELD)
		c = 'M';
	else if (flags & SEXTERN)
		c = 'X';
	else
		c = 'A';
	fprintf(outfp, "%c%u", c, sym->id);
}

static void
emitconst(Node *np)
{
	Symbol *sym = np->sym;
	Type *tp = np->type;
	TUINT u;

	switch (tp->op) {
	case PTR:
	case INT:
	case ENUM:
		u = (tp->prop & TSIGNED) ? (TUINT) sym->u.i : sym->u.u;
		fprintf(outfp,
		        "#%c%llX",
		        np->type->letter,
		        (long long) u & ones(tp->size));
		break;
	default:
		abort();
	}
}

static void
emitsym(int op, void *arg)
{
	Node *np = arg;

	if ((np->sym->flags & SINITLST) == 0) {
		/*
		 * When we have a compound literal we are going
		 * to call to emitnode for every element of it,
		 * and it means that we will have two '\t'
		 * for the first element
		 */
		putc('\t', outfp);
	}
	(np->flags & NCONST) ? emitconst(np) : emitvar(np->sym);
}

static void
emitletter(Type *tp)
{
	int letter;

	letter = (tp->prop&TELLIPSIS) ? 'E' : tp->letter;
	putc(letter, outfp);
	switch (tp->op) {
	case ARY:
	case STRUCT:
	case UNION:
		fprintf(outfp, "%u", tp->id);
	}
}

static void
emittype(int op, void *arg)
{
	TINT n;
	Symbol **sp;
	char *tag;
	Type *tp = arg;

	if (!(tp->prop & TDEFINED))
		return;

	switch (tp->op) {
	case ARY:
		emitletter(tp);
		putc('\t', outfp);
		emitletter(tp->type);
		fprintf(outfp,
		        "\t#%c%llX\n",
		        sizettype->letter, (long long) tp->n.elem);
		return;
	case UNION:
	case STRUCT:
		emitletter(tp);
		tag = tp->tag->name;
		fprintf(outfp,
		       "\t\"%s\t#%c%lX\t#%c%X\n",
		       (tag) ? tag : "",
		       sizettype->letter,
		       tp->size,
		       sizettype->letter,
		       tp->align);
		n = tp->n.elem;
		for (sp = tp->p.fields; n-- > 0; ++sp)
			emit(ODECL, *sp);
		break;
	case PTR:
	case FTN:
	case ENUM:
		return;
	default:
		abort();
	}
}

static void
emitstring(Symbol *sym, Type *tp)
{
	char *bp, *s, *lim;
	int n;

	bp = sym->u.s;
	lim = &sym->u.s[tp->n.elem];
	while (bp < lim) {
		s = bp;
		while (bp < lim && isprint(*bp))
			++bp;
		if ((n = bp - s) > 1)
			fprintf(outfp, "\t#\"%.*s\n", n, s);
		else
			bp = s;
		if (bp == lim)
			break;
		do {
			fprintf(outfp,
			        "\t#%c%02X\n",
			        chartype->letter, (*bp++) & 0xFF);
		} while (bp < lim && !isprint(*bp));
	}
}

static Node *
zeronode(Type *tp)
{
	return simplify(convert(constnode(zero), tp, 0));
}

static int
emitpadding(Type *tp, SIZET *addr)
{
	SIZET n;
	int i;

	n = *addr & tp->align-1;
	for (i = 0; i < n; i++)
		emitexp(OEXPR, zeronode(chartype));
	*addr += n;

	return n;
}

static void
emitdesig(Node *np, Type *tp, SIZET *addr)
{
	Symbol *sym;
	SIZET n;
	Node *aux;
	Type *p;

	emitpadding(tp, addr);

	if (!np) {
		sym = NULL;
	} else {
		if (!np->sym)
			goto emit_expression;
		sym = np->sym;
		if (sym->flags & SSTRING) {
			emitstring(sym, tp);
			*addr += tp->n.elem;
			return;
		}
		if ((sym->flags & SINITLST) == 0)
			goto emit_expression;
	}

	switch (tp->op) {
	case PTR:
	case INT:
	case ENUM:
		aux = sym ? *sym->u.init : zeronode(tp);
		*addr += aux->type->size;
		emitexp(OEXPR, aux);
		break;
	case UNION:
		aux = (sym) ? sym->u.init[0] : NULL;
		emitdesig(aux, aux->type, addr);
		emitpadding(tp, addr);
		break;
	case STRUCT:
	case ARY:
		for (n = 0; n < tp->n.elem; ++n) {
			aux = (sym) ? sym->u.init[n] : NULL;
			p = (tp->op == ARY) ? tp->type : tp->p.fields[n]->type;
			emitdesig(aux, p, addr);
		}
		emitpadding(tp, addr);
		break;
	default:
		abort();
	}

	if (sym) {
		free(sym->u.init);
		sym->u.init = NULL;
	}
	freetree(np);
	return;

emit_expression:
	emitexp(OEXPR, np);
	*addr += tp->size;
}

static void
emitinit(int op, void *arg)
{
	Node *np = arg;
	SIZET addr = 0;

	fputs("\t(\n", outfp);
	emitdesig(np, np->type, &addr);
	fputs(")\n", outfp);
}

static void
emitdcl(int op, void *arg)
{
	Symbol *sym = arg;

	if (sym->flags & SEMITTED)
		return;
	emitvar(sym);
	putc('\t', outfp);
	if (sym->type->op == FTN) {
		emitletter(sym->type->type);
		putc('\t', outfp);
	}
	emitletter(sym->type);
	fprintf(outfp, "\t\"%s", (sym->name) ? sym->name : "");
	if (sym->flags & SFIELD)
		fprintf(outfp, "\t#%c%llX", sizettype->letter, sym->u.i);
	sym->flags |= SEMITTED;
	if ((sym->flags & SHASINIT) == 0)
		putc('\n', outfp);
}

static void
emitcast(int op, void *arg)
{
	Node *np = arg, *lp = np->left;

	emitnode(lp);
	if (np->type != voidtype)
		fprintf(outfp, "\tg%c", np->type->letter);
}

static void
emitbin(int op, void *arg)
{
	Node *np = arg;
	char *s;

	emitnode(np->left);
	emitnode(np->right);

	/* do not print in OCOLON case */
	if ((s = optxt[op]) != NULL)  {
		fprintf(outfp, "\t%s", s);
		emitletter(np->type);
	}
}

static void
emitbuilt(int op, void *arg)
{
	Node *np = arg;

	emitnode(np->left);
	emitnode(np->right);
	fprintf(outfp, "\t\"%s\tm", np->sym->name);
	emitletter(np->type);
}


static void
emitexp(int op, void *arg)
{
	Node *np = arg;

	emitnode(np);
	putc('\n', outfp);
	freetree(np);
}

static void
emitfun(int op, void *arg)
{
	Symbol *sym = arg, **sp;

	emitdcl(op, arg);
	fputs("{\n", outfp);

	for (sp = sym->u.pars; sp && *sp; ++sp)
		emit(ODECL, *sp);
	fputs("\\\n", outfp);
}

static void
emittext(int op, void *arg)
{
	fputs(optxt[op], outfp);
}

static void
emitsymid(int op, void *arg)
{
	Symbol *sym = arg;
	fprintf(outfp, optxt[op], sym->id);
}

Node *
node(int op, Type *tp, Node *lp, Node *rp)
{
	Node *np;

	np = xmalloc(sizeof(*np));
	np->op = op;
	np->type = tp;
	np->sym = NULL;
	np->flags = 0;
	np->left = lp;
	np->right = rp;

	if (lp)
		np->flags |= lp->flags & NEFFECT;
	if (rp)
		np->flags |= rp->flags & NEFFECT;
	return np;
}

Node *
varnode(Symbol *sym)
{
	Node *np;
	Type *tp = sym->type;

	np = node(OSYM, sym->type, NULL, NULL);
	np->type = sym->type;
	np->flags = (tp->op != FTN && tp->op != ARY) ? NLVAL : 0;
	np->sym = sym;
	return np;
}

Node *
constnode(Symbol *sym)
{
	Node *np;

	np = node(OSYM, sym->type, NULL, NULL);
	np->flags = NCONST;
	np->sym = sym;
	return np;
}

Node *
sizeofnode(Type *tp)
{
	Symbol *sym;

	sym = newsym(NS_IDEN, NULL);
	sym->type = sizettype;
	sym->u.u = tp->size;
	DBG("EXPR sizeof %llu", sym->u.u);
	return constnode(sym);
}

Node *
offsetnode(Symbol *field, Type *tp)
{
	Symbol *sym;

	assert(field->flags & SFIELD);
	sym = newsym(NS_IDEN, NULL);
	sym->type = tp;
	sym->flags |= SCONSTANT;
	sym->u.u = field->u.u;

	return constnode(sym);
}
