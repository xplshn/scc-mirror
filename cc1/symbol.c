
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/cc.h"
#include "../inc/sizes.h"
#include "cc1.h"

#define NR_SYM_HASH 64

unsigned curctx;
static short localcnt;
static short globalcnt;

static Symbol *head;
static Symbol *htab[NR_SYM_HASH];

#ifndef NDEBUG
void
dumpstab(char *msg)
{
	Symbol **bp, *sym;

	fprintf(stderr, "%s\n", msg);
	for (bp = htab; bp < &htab[NR_SYM_HASH]; ++bp) {
		if (*bp == NULL)
			continue;
		fprintf(stderr, "%d", (int) (bp - htab));
		for (sym = *bp; sym; sym = sym->hash)
			fprintf(stderr, "->%d:%s", sym->ns, sym->name);
		putc('\n', stderr);
	}
}
#endif

static unsigned
hash(const char *s)
{
	unsigned c, h;

	for (h = 0; c = *s; ++s)
		h ^= 33 * c;
	return h & NR_SYM_HASH-1;
}

void
pushctx(void)
{
	if (++curctx == NR_BLOCK)
		error("too much nested blocks");
}

void
popctx(void)
{
	Symbol *next, *sym;
	Symbol dummy = {.next = NULL}, *hp = &dummy;

	if (--curctx == 0)
		localcnt = 0;

	for (sym = head; sym && sym->ctx > curctx; sym = next) {
		next = sym->next;
		switch (sym->ns) {
		case NS_LABEL:
			if (curctx != 0)
				goto save_symbol;
			if (sym->flags & ISDECLARED)
				break;
			printerr("label '%s' is not defined", sym->name);
			break;
		case NS_CPP:
		save_symbol:
			/*
			 * CPP symbols have file scope
			 * Labels have function scope
			 */
			hp->next = sym;
			hp = sym;
			continue;
		case NS_TAG:
			sym->type->defined = 0;
			break;
		}
		if (sym->name) {
			short f = sym->flags;
			htab[hash(sym->name)] = sym->hash;
			if ((f & (ISUSED|ISGLOBAL|ISDECLARED)) == ISDECLARED)
				warn("'%s' defined but not used", sym->name);
		}
		free(sym->name);
		// TODO: Fix this memory leak free(sym->u.pars);
		free(sym);
	}
	hp->next = sym;
	head = dummy.next;
}

Type *
duptype(Type *base)
{
	Type *tp = xmalloc(sizeof(*tp));

	*tp = *base;
	tp->id = (curctx) ? ++localcnt : ++globalcnt;
	return tp;
}

Symbol *
newsym(unsigned ns)
{
	Symbol *sym;

	sym = malloc(sizeof(*sym));
	sym->id = 0;
	sym->ns = ns;
	sym->ctx = curctx;
	sym->token = IDEN;
	sym->flags = ISDECLARED;
	sym->name = NULL;
	sym->type = NULL;
	sym->hash = NULL;

	if (!head || head->ctx <= curctx) {
		sym->next = head;
		head = sym;
	} else {
		Symbol *p, *prev;

		for (prev = p = head; p; prev = p, p = p->next) {
			if (p->ctx <= sym->ctx)
				break;
		}
		p = prev->next;
		prev->next = sym;
		sym->next = p;
	}
	return sym;
}

Symbol *
lookup(unsigned ns)
{
	Symbol *sym, **h;
	unsigned sns;
	char *t, c;

	h = &htab[hash(yytext)];
	c = *yytext;
	for (sym = *h; sym; sym = sym->hash) {
		t = sym->name;
		if (*t != c || strcmp(t, yytext))
			continue;
		sns = sym->ns;
		if (sns == NS_KEYWORD || sns == NS_CPP)
			return sym;
		if (sns != ns)
			continue;
		return sym;
	}

	sym = newsym(ns);
	sym->name = xstrdup(yytext);
	sym->flags &= ~ISDECLARED;
	sym->hash = *h;
	*h = sym;
	return sym;
}

Symbol *
nextsym(Symbol *sym, unsigned ns)
{
	char *s, *t, c;
	Symbol *new, *p;

	/* FIXME:
	 * This function is only called when a macro with parameters
	 * is called without them.
	 *      #define x(y) ((y) + 1)
	 *      int x = x(y);
	 * This solution fixes the problem but destroy the order of
	 * contexts in the hash table.
	 */
	s = sym->name;
	c = *s;
	for (p = sym->hash; p; p = p->hash) {
		t = p->name;
		if (c == *t && !strcmp(s, t))
			return sym;
	}
	new = newsym(ns);
	new->flags &= ~ISDECLARED;
	new->name = xstrdup(yytext);
	new->hash = sym->hash;
	return sym->hash = new;
}

Symbol *
install(unsigned ns, Symbol *sym)
{
	if (sym->ctx == curctx) {
		if (sym->flags & ISDECLARED)
			return NULL;
		sym->flags |= ISDECLARED;
	} else {
		char *name = sym->name;
		Symbol **h;

		sym = newsym(ns);
		sym->name = xstrdup(name);
		h = &htab[hash(name)];

		if (!*h || (*h)->ctx <= curctx) {
			sym->hash = *h;
			*h = sym;
		} else {
			Symbol *p, *prev;

			for (prev = p = *h; p; prev = p, p = p->hash) {
				if (p->ctx <= sym->ctx)
					break;
			}
			p = prev->hash;
			prev->hash = sym;
			sym->hash = p;
		}
	}

	if (sym->ns != NS_CPP)
		sym->id = (curctx) ? ++localcnt : ++globalcnt;

	return sym;
}

void
ikeywords(void)
{
	static struct {
		char *str;
		unsigned char token, value;
	} *bp, keywords[] = {
		{"auto", SCLASS, AUTO},
		{"break", BREAK, BREAK},
		{"_Bool", TYPE, BOOL},
		{"case", CASE, CASE},
		{"char", TYPE, CHAR},
		{"const", TQUALIFIER, CONST},
		{"continue", CONTINUE, CONTINUE},
		{"default", DEFAULT, DEFAULT},
		{"do", DO, DO},
		{"double", TYPE, DOUBLE},
		{"else", ELSE, ELSE},
		{"enum", TYPE, ENUM},
		{"extern", SCLASS, EXTERN},
		{"float", TYPE, FLOAT},
		{"for", FOR, FOR},
		{"goto", GOTO, GOTO},
		{"if", IF, IF},
		{"int", TYPE, INT},
		{"long", TYPE, LONG},
		{"register", SCLASS, REGISTER},
		{"restrict", TQUALIFIER, RESTRICT},
		{"return", RETURN, RETURN},
		{"short", TYPE, SHORT},
		{"signed", TYPE, SIGNED},
		{"sizeof", SIZEOF, SIZEOF},
		{"static", SCLASS, STATIC},
		{"struct", TYPE, STRUCT},
		{"switch", SWITCH, SWITCH},
		{"typedef", SCLASS, TYPEDEF},
		{"union", TYPE, UNION},
		{"unsigned", TYPE, UNSIGNED},
		{"void", TYPE, VOID},
		{"volatile", TQUALIFIER, VOLATILE},
		{"while", WHILE, WHILE},
		{NULL, 0, 0},
	}, cppclauses[] = {
		{"define", DEFINE, DEFINE},
		{"include", INCLUDE, INCLUDE},
		{"line", LINE, LINE},
		{"ifdef", IFDEF, IFDEF},
		{"if", IF, IF},
		{"elif", ELIF, ELIF},
		{"else", ELSE, ELSE},
		{"ifndef", IFNDEF, IFNDEF},
		{"endif", ENDIF, ENDIF},
		{"undef", UNDEF, UNDEF},
		{"pragma", PRAGMA, PRAGMA},
		{"error", ERROR, ERROR},
		{NULL, 0, 0}
	}, *list[] = {
		keywords,
		cppclauses,
		NULL
	}, **lp;
	Symbol *sym;
	int ns = NS_KEYWORD;

	for (lp = list; *lp; ++lp) {
		for (bp = *lp; bp->str; ++bp) {
			strcpy(yytext, bp->str);
			sym = lookup(ns);
			sym->token = bp->token;
			sym->u.token = bp->value;
		}
		head = NULL;
		ns = NS_CPPCLAUSES;
	}
}
