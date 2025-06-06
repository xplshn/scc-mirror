#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <scc/cstd.h>
#include <scc/scc.h>
#include "cc1.h"

#define NOSCLASS  0

#define NOREP 0
#define REP 1
#define QUIET   1
#define NOQUIET 0

#define NR_DCL_TYP (NR_DECLARATORS+NR_FUNPARAM)
#define NR_DCL_SYM (NR_DECLARATORS+NR_FUNPARAM+1)

struct declarators {
	unsigned nr;
	unsigned ns;
	struct decl *dcl;
	unsigned nr_types;
	Type **tpars;
	Symbol **pars;
	struct declarator {
		unsigned char op;
		TINT  nelem;
		Symbol *sym;
		Type **tpars;
		Symbol **pars;
	} d [NR_DECLARATORS];
};

struct decl {
	unsigned ns;
	int sclass;
	int qualifier;
	Symbol *sym;
	Type *type;
	Type *parent;
	Symbol **pars;
	Symbol *bufpars[NR_DCL_SYM];
	Type *buftpars[NR_DCL_TYP];
};


static void
endfundcl(Type *tp, Symbol **pars)
{
	/*
	 * If endfundcl is called from a type built from a typedef then
	 * we do not have any parameters because in that case we only
	 * care about the type.
	 */
	if (pars) {
		if ((tp->prop&TK_R) != 0 && *pars)
			warn("parameter names (without types) in function declaration");

		/* avoid non used warnings in prototypes */
		while (*pars)
			(*pars++)->flags |= SUSED;
		popctx();
	}
}

static void
push(struct declarators *dp, int op, ...)
{
	va_list va;
	unsigned n;
	struct declarator *p;

	va_start(va, op);
	if ((n = dp->nr++) == NR_DECLARATORS)
		error("too many declarators");

	p = &dp->d[n];
	p->op = op;
	p->tpars = NULL;

	switch (op) {
	case ARY:
		p->nelem = va_arg(va, TINT);
		break;
	case KRFTN:
	case FTN:
		p->nelem = va_arg(va, unsigned);
		p->tpars = va_arg(va, Type **);
		p->pars = va_arg(va, Symbol **);
		break;
	case IDEN:
		p->sym = va_arg(va, Symbol *);
		break;
	}
	va_end(va);
}

static int
pop(struct declarators *dp, struct decl *dcl)
{
	struct declarator *p;

	if (dp->nr == 0)
		return 0;

	p = &dp->d[--dp->nr];
	if (p->op == IDEN) {
		dcl->sym = p->sym;
		return 1;
	}

	/*
	 * We have a type derived from a function type. We don't care
	 * about the parameters because they were used only in the
	 * process of building a final type. Prototype arguments are
	 * discarded in funbody() because the final type of the decl
	 * is an actual function.
	 */
	if (dcl->type->op == FTN)
		endfundcl(dcl->type, dcl->pars);
	dcl->pars = p->pars;

	dcl->type = mktype(dcl->type, p->op, p->nelem, p->tpars);
	return 1;
}

static void
arydcl(struct declarators *dp)
{
	Node *np = NULL;
	TINT n = 0;
	int ns;

	ns = namespace;
	namespace = NS_IDEN;
	expect('[');
	if (yytoken != ']') {
		if ((np = constexpr()) == NULL) {
			errorp("invalid storage size");
		} else {
			if ((n = np->sym->u.i) <= 0) {
				errorp("array size is not a positive number");
				n = 1;
			}
			freetree(np);
		}
	}
	namespace = ns;
	expect(']');

	push(dp, ARY, n);
}

static int
empty(Symbol *sym, Type *tp, int param)
{
	if (!sym->name) {
		sym->type = tp;
		switch (tp->op) {
		default:
			 /* warn if it is not a parameter */
			if (!param)
				warn("empty declaration");
		case STRUCT:
		case UNION:
		case ENUM:
			return 1;
		}
	}
	return 0;
}

static void
bad_storage(Type *tp, char *name)
{
	if (tp->op != FTN)
		errorp("incorrect storage class for file-scope declaration");
	else
		errorp("invalid storage class for function '%s'", name);
}

static Symbol *
redcl(Symbol *sym, Type *tp, int sclass)
{
	int flags;
	char *name = sym->name;

	if (!eqtype(sym->type, tp, EQUIV)) {
		errorp("conflicting types for '%s'", name);
		return sym;
	}

	/* we prefere ansi types over k&r types */
	if ((sym->type->prop & TK_R) == 0 && (tp->prop & TK_R) != 0)
		sym->type = tp;

	if (sym->token == TYPEIDEN && sclass != TYPEDEF ||
	    sym->token != TYPEIDEN && sclass == TYPEDEF) {
		goto redeclaration;
	}
	if (curctx != GLOBALCTX && tp->op != FTN) {
		/* is it the redeclaration of a local variable? */
		if ((sym->flags & SEXTERN) && sclass == EXTERN)
			return sym;
		goto redeclaration;
	}

	flags = sym->flags;
	switch (sclass) {
	case REGISTER:
	case AUTO:
		bad_storage(tp, name);
		break;
	case NOSCLASS:
		if ((flags & SPRIVATE) == 0) {
			if (flags & SEXTERN)
				flags &= ~(SEXTERN|SEMITTED);
			flags |= SGLOBAL;
			break;
		}
		errorp("non-static declaration of '%s' follows static declaration",
		       name);
		break;
	case TYPEDEF:
		/* Only C11 allows multiple definitions of a typedef. */
		goto redeclaration;
	case EXTERN:
		break;
	case STATIC:
		if ((flags & (SGLOBAL|SEXTERN)) == 0) {
			flags |= SPRIVATE;
			break;
		}
		errorp("static declaration of '%s' follows non-static declaration",
		       name);
		break;
	}
	sym->flags = flags;

	return sym;

redeclaration:
	errorp("redeclaration of '%s'", name);
	return sym;
}

static Symbol *
identifier(struct decl *dcl)
{
	Symbol *sym = dcl->sym;
	Type *tp = dcl->type;
	int sclass = dcl->sclass;
	char *name = sym->name;

	if (empty(sym, tp, 0))
		return sym;

	/* TODO: Add warning about ANSI limits */
	if (!(tp->prop & TDEFINED)                &&
	    sclass != EXTERN && sclass != TYPEDEF &&
	    !(tp->op == ARY && yytoken == '=')) {
		errorp("declared variable '%s' of incomplete type", name);
	}

	if (tp->op == FTN) {
		if (sclass == NOSCLASS)
			sclass = EXTERN;
		if (!strcmp(name, "main") && tp->type != inttype) {
			errorp("main shall be defined with a return type of int");
		}
	}

	if (strcmp(name, "__func__") == 0)
		errorp("__func__ is a reserved variable name");

	if (sym->flags & SDECLARED) {
		sym = redcl(dcl->sym, tp, sclass);
	} else {
		int flags = sym->flags | SDECLARED;

		sym->type = tp;

		switch (sclass) {
		case REGISTER:
		case AUTO:
			if (curctx != GLOBALCTX && tp->op != FTN) {
				flags |= (sclass == REGISTER) ? SREGISTER : SAUTO;
				break;
			}
			bad_storage(tp, name);
		case NOSCLASS:
			if (tp->op == FTN)
				flags |= SEXTERN;
			else
				flags |= (curctx == GLOBALCTX) ? SGLOBAL : SAUTO;
			break;
		case EXTERN:
			flags |= SEXTERN;
			break;
		case STATIC:
			flags |= (curctx == GLOBALCTX) ? SPRIVATE : SLOCAL;
			break;
		case TYPEDEF:
			flags |= STYPEDEF;
			sym->u.token = sym->token = TYPEIDEN;
			break;
		}
		sym->flags = flags;
	}

	if (accept('='))
		initializer(sym, sym->type);
	if (!(sym->flags & (SGLOBAL|SEXTERN)) && tp->op != FTN)
		sym->flags |= SDEFINED;
	if (sym->token == IDEN && tp->op != FTN)
		emit(ODECL, sym);
	return sym;
}

static Symbol *
parameter(struct decl *dcl)
{
	Symbol *sym = dcl->sym;
	Type *funtp = dcl->parent, *tp = dcl->type;
	char *name = sym->name;
	int flags;

	flags = 0;
	switch (dcl->sclass) {
	case STATIC:
	case EXTERN:
	case AUTO:
		errorp("bad storage class in function parameter");
		break;
	case REGISTER:
		flags |= SREGISTER;
		break;
	case NOSCLASS:
		flags |= SAUTO;
		break;
	}

	switch (tp->op) {
	case VOID:
		funtp->n.elem = 1;
		if (dcl->sclass)
			errorp("void as unique parameter may not be qualified");
		return NULL;
	case ARY:
		tp = mktype(tp->type, PTR, 0, NULL);
		break;
	case FTN:
		errorp("incorrect function type for a function parameter");
		return NULL;
	}
	if (!empty(sym, tp, 1)) {
		int isdcl = sym->flags&SDECLARED, isk_r = funtp->prop & TK_R;
		if (isdcl && !isk_r) {
			errorp("redefinition of parameter '%s'", name);
			return NULL;
		}
		if (!isdcl && isk_r) {
			errorp("declaration for parameter '%s' but no such parameter",
			       sym->name);
			return NULL;
		}
		if (strcmp(name, "__func__") == 0)
			errorp("__func__ is a reserved variable name");
		sym->flags |= SDECLARED;
	}

	sym->type = tp;
	sym->flags &= ~(SAUTO|SREGISTER);
	sym->flags |= flags;
	return sym;
}

static Symbol *dodcl(int rep,
                     Symbol *(*fun)(struct decl *),
                     unsigned ns,
                     Type *type);

static int
krpars(struct declarators *dp)
{
	Symbol *sym;
	int toomany = 0;
	unsigned npars = 0;

	do {
		sym = yylval.sym;
		expect(IDEN);
		sym->flags |= SAUTO;
		if ((sym = install(NS_IDEN, sym)) == NULL) {
			errorp("redefinition of parameter '%s'",
			       yylval.sym->name);
			continue;
		}
		if (npars < NR_FUNPARAM) {
			++npars;
			*dp->pars++ = sym;
			continue;
		}
		toomany = 1;
	} while (accept(','));

	return toomany;
}

static unsigned
krfun(struct declarators *dp)
{
	int toomany = 0;

	if (yytoken != ')')
		toomany = krpars(dp);

	if (dp->nr_types == NR_DCL_TYP) {
		toomany = 1;
	} else {
		++dp->nr_types;
		*dp->tpars++ = ellipsistype;
	}

	if (toomany)
		errorp("too many parameters in function definition");
	return 1;
}

static unsigned
ansifun(struct declarators *dp)
{
	Symbol *sym;
	unsigned npars, ntype, toomany, distoomany, voidpar;
	Type type, *tp;

	type.n.elem = 0;
	type.prop = 0;
	npars = ntype = toomany = distoomany = voidpar = 0;

	do {
		if (accept(ELLIPSIS)) {
			if (ntype < 1)
				errorp("a named argument is requiered before '...'");
			if (yytoken != ')')
				errorp("... must be the last parameter");
			sym = NULL;
			tp = ellipsistype;
		} else if ((sym = dodcl(NOREP, parameter, NS_IDEN, &type)) == NULL) {
			voidpar = 1;
			sym = NULL;
			tp = NULL;
		} else {
			tp = sym->type;
		}

		if (sym) {
			if (npars == NR_FUNPARAM) {
				toomany = 1;
			} else {
				npars++;
				*dp->pars++ = sym;
			}
		}

		if (tp) {
			if (dp->nr_types == NR_DCL_TYP) {
				toomany = 1;
			} else {
				ntype++;
				dp->nr_types++;
				*dp->tpars++ = tp;
			}
		}

	} while (accept(','));

	if (toomany == 1)
		errorp("too many parameters in function definition");
	if (voidpar && ntype > 0)
		errorp("'void' must be the only parameter");
	return ntype;
}

static int
isfunbody(int tok)
{
	switch (tok) {
	case '{':
	case TYPE:
	case SCLASS:
	case TYPEIDEN:
		return 1;
	default:
		return 0;
	}
}

static int
funbody(Symbol *sym, Symbol *pars[])
{
	Type *tp;
	Symbol **bp, *p;
	Symbol *emptypars[] = {NULL};

	if (!sym)
		return 0;

	tp = sym->type;
	if (tp->op != FTN)
		return 0;
	if (!isfunbody(yytoken) || sym->ns != NS_IDEN) {
		emit(ODECL, sym);
		endfundcl(tp, pars);
		return  0;
	}

	if (curctx < PARAMCTX) {
		assert(!pars);
		errorp("typedef'ed function type cannot be instantiated");
		curctx = PARAMCTX;
		pars = emptypars;
	}

	if (curctx != PARAMCTX)
		error("nested function declaration");

	tp->prop |= TFUNDEF;
	curfun = sym;
	if (tp->prop & TK_R) {
		while (yytoken != '{') {
			dodcl(REP, parameter, NS_IDEN, sym->type);
			expect(';');
		}
		for (bp = pars; p = *bp; ++bp) {
			if (p->type == NULL) {
				warn("type of '%s' defaults to int", p->name);
				p->type = inttype;
			}
		}
	}
	if (sym->flags & STYPEDEF)
		errorp("function definition declared 'typedef'");
	if (sym->flags & SDEFINED)
		errorp("redefinition of '%s'", sym->name);
	if (sym->flags & SEXTERN) {
		sym->flags &= ~SEXTERN;
		sym->flags |= SGLOBAL;
	}
	sym->flags |= SDEFINED;
	sym->flags &= ~SEMITTED;
	sym->u.pars = pars;
	emit(OFUN, sym);
	compound(NULL, NULL, NULL);
	emit(OEFUN, NULL);
	popctx();
	flushtypes();
	curfun = NULL;

	/*
	 * A function declaration without arguments is a k&r function,
	 * but when it is a definition is a function with 0 arguments
	 */
	if ((tp->prop & TK_R) && *pars == NULL) {
		tp = mktype(tp->type, FTN, 0, NULL);
		tp->prop |= TFUNDEF;
		sym->type = tp;
	}

	return 1;
}

static void
fundcl(struct declarators *dp)
{
	Type **types = dp->tpars;
	unsigned ntypes, typefun;
	Symbol **pars = dp->pars;
	unsigned (*fun)(struct declarators *);

	pushctx();
	expect('(');
	if (yytoken == ')' || yytoken == IDEN) {
		typefun = KRFTN;
		fun = krfun;
	} else {
		typefun = FTN;
		fun = ansifun;
	}
	ntypes = (*fun)(dp);
	*dp->pars++= NULL;
	expect(')');

	push(dp, typefun, ntypes, types, pars);
}

static void declarator(struct declarators *dp);

static void
directdcl(struct declarators *dp)
{
	Symbol *p, *sym;
	static int nested;

	if (accept('(')) {
		if (nested == NR_SUBTYPE)
			error("too many declarators nested by parentheses");
		++nested;
		declarator(dp);
		--nested;
		expect(')');
	} else {
		if (yytoken == IDEN || yytoken == TYPEIDEN) {
			sym = yylval.sym;
			if (p = install(dp->ns, sym)) {
				sym = p;
				sym->flags &= ~SDECLARED;
			}
			next();
		} else {
			sym = newsym(dp->ns, NULL);
		}
		push(dp, IDEN, sym);
	}

	for (;;) {
		switch (yytoken) {
		case '(':  fundcl(dp); break;
		case '[':  arydcl(dp); break;
		default:   return;
		}
	}
}

static void
declarator(struct declarators *dp)
{
	unsigned  n;

	for (n = 0; accept('*'); ++n) {
		while (accept(TQUALIFIER))
			;
	}

	directdcl(dp);

	while (n--)
		push(dp, PTR);
}

static Type *structdcl(void), *enumdcl(void);

static Type *
specifier(int *sclass, int *qualifier)
{
	Type *tp = NULL;
	unsigned spec, qlf, sign, type, cls, size;

	spec = qlf = sign = type = cls = size = 0;

	for (;;) {
		unsigned *p = NULL;
		Type *(*dcl)(void) = NULL;

		switch (yytoken) {
		case SCLASS:
			p = &cls;
			break;
		case TQUALIFIER:
			qlf |= yylval.token;
			next();
			continue;
		case TYPEIDEN:
			if (type)
				goto return_type;
			tp = yylval.sym->type;
			p = &type;
			break;
		case TYPE:
			switch (yylval.token) {
			case ENUM:
				dcl = enumdcl;
				p = &type;
				break;
			case STRUCT:
			case UNION:
				dcl = structdcl;
				p = &type;
				break;
			case VA_LIST:
			case VOID:
			case BOOL:
			case CHAR:
			case INT:
			case FLOAT:
			case DOUBLE:
				p = &type;
				break;
			case SIGNED:
			case UNSIGNED:
				p = &sign;
				break;
			case LONG:
				if (size == LONG) {
					yylval.token = LLONG;
					size = 0;
				}
			case SHORT:
				p = &size;
				break;
			}
			break;
		default:
			goto return_type;
		}
		if (*p)
			errorp("invalid type specification");
		*p = yylval.token;
		if (dcl) {
			if (size || sign)
				errorp("invalid type specification");
			tp = (*dcl)();
			goto return_type;
		} else {
			next();
		}
		spec = 1;
	}

return_type:
	*sclass = cls;
	*qualifier = qlf;
	if (!tp) {
		if (spec) {
			tp = ctype(type, sign, size);
		} else {
			if (curctx != GLOBALCTX)
				unexpected();
			warn("type defaults to 'int' in declaration");
			tp = inttype;
		}
	}
	return tp;
}

static Symbol *
newtag(void)
{
	Symbol *sym;
	int ns, op, tag = yylval.token;
	static unsigned tpns = NS_STRUCTS;

	ns = namespace;
	namespace = NS_TAG;
	next();
	namespace = ns;

	switch (yytoken) {
	case IDEN:
	case TYPEIDEN:
		sym = yylval.sym;
		if ((sym->flags & SDECLARED) == 0)
			install(NS_TAG, yylval.sym);
		next();
		break;
	default:
		sym = newsym(NS_TAG, NULL);
		break;
	}
	if (!sym->type) {
		Type *tp;

		if (tpns == NS_STRUCTS + NR_MAXSTRUCTS)
			error("too many tags declared");
		tp = mktype(NULL, tag, 0, NULL);
		tp->ns = tpns++;
		sym->type = tp;
		tp->tag = sym;
		DBG("DECL: declared tag '%s' with ns = %d\n",
		    (sym->name) ? sym->name : "anonymous", tp->ns);
	}

	if ((op = sym->type->op) != tag &&  op != INT)
		error("'%s' defined as wrong kind of tag", sym->name);
	return sym;
}

static void fieldlist(Type *tp);

static Type *
structdcl(void)
{
	Symbol *sym;
	Type *tp;
	static int nested;
	int ns;

	sym = newtag();
	tp = sym->type;

	if (!accept('{'))
		return tp;

	ns = namespace;
	namespace = tp->ns;

	if (tp->prop & TDEFINED && sym->ctx == curctx)
		error("redefinition of struct/union '%s'", sym->name);

	if (nested == NR_STRUCT_LEVEL)
		error("too many levels of nested structure or union definitions");

	++nested;
	do fieldlist(tp); while (yytoken != '}');
	--nested;

	deftype(tp);
	namespace = ns;
	expect('}');
	return tp;
}

static Type *
enumdcl(void)
{
	Type *tp;
	Symbol *sym, *tagsym;
	int ns, val, toomany;
	unsigned nctes;

	ns = namespace;
	tagsym = newtag();
	tp = tagsym->type;

	namespace = NS_IDEN;
	if (!accept('{')) {
		namespace = ns;
		return tp;
	}
	if (tp->prop & TDEFINED)
		errorp("redefinition of enumeration '%s'", tagsym->name);
	deftype(tp);

	/* TODO: check incorrect values in val */
	for (nctes = val = 0; yytoken != '}'; ++nctes, ++val) {
		if (yytoken != IDEN)
			unexpected();
		sym = yylval.sym;
		next();
		if (nctes == NR_ENUM_CTES && !toomany) {
			errorp("too many enum constants in a single enum");
			toomany = 1;
		}
		if (accept('=')) {
			Node *np = constexpr();

			if (np == NULL)
				errorp("invalid enumeration value");
			else
				val = np->sym->u.i;
			freetree(np);
		}
		if ((sym = install(NS_IDEN, sym)) == NULL) {
			errorp("'%s' redeclared as different kind of symbol",
			       yytext);
		} else {
			sym->u.i = val;
			sym->flags |= SCONSTANT;
			sym->type = inttype;
		}
		if (!accept(','))
			break;
	}
	namespace = ns;
	expect('}');
	return tp;
}

static Symbol *
type(struct decl *dcl)
{
	Symbol *sym = dcl->sym;

	if (dcl->sclass)
		error("class storage in type name");
	if (sym->name)
		error("unexpected identifier in type name");
	sym->type = dcl->type;

	return sym;
}

static Symbol *
field(struct decl *dcl)
{
	static char *anon = "<anonymous>";
	Symbol *sym = dcl->sym;
	char *name = (sym->name) ? sym->name : anon;
	Type *structp = dcl->parent, *tp = dcl->type;
	TINT n = structp->n.elem;

	if (accept(':')) {
		Node *np;
		TINT n;

		if ((np = constexpr()) == NULL) {
			unexpected();
			n = 0;
		} else {
			n = np->sym->u.i;
			freetree(np);
		}
		if (n == 0 && name != anon)
			errorp("zero width for bit-field '%s'", name);
		if (tp != booltype && tp != inttype && tp != uinttype)
			errorp("bit-field '%s' has invalid type", name);
		if (n < 0)
			errorp("negative width in bit-field '%s'", name);
		else if (n > tp->size*8)
			errorp("width of '%s' exceeds its type", name);
	} else if (empty(sym, tp, 0)) {
		return sym;
	}

	if (sym->flags & SDECLARED) {
		errorp("duplicated member '%s'", name);
		return sym;
	}

	if ((tp->prop & TDEFINED) == 0) {
		if (tp->op == ARY && tp->n.elem == 0) {
			if (n == 0)
				errorp("flexible array member in a struct with no named members");
			if (ahead() != '}')
				errorp("flexible array member not at end of struct");
		} else {
			errorp("field '%s' has incomplete type", name);
			tp = inttype;
		}
	}
	if (tp->op == FTN) {
		errorp("field '%s' declared as a function", name);
		tp = inttype;
	}
	if (dcl->sclass)
		errorp("storage class in struct/union field '%s'", name);

	sym->type = tp;
	sym->flags |= SFIELD|SDECLARED;

	if (n == NR_FIELDS) {
		errorp("too many fields in struct/union");
		return sym;
	}

	DBG("DECL: New field '%s' in namespace %d\n", name, structp->ns);
	structp->p.fields = xrealloc(structp->p.fields, ++n * sizeof(*sym));
	structp->p.fields[n-1] = sym;
	structp->n.elem = n;

	return sym;
}

static Symbol *
dodcl(int rep, Symbol *(*fun)(struct decl *), unsigned ns, Type *parent)
{
	Symbol *sym;
	Type *base;
	struct decl dcl;
	struct declarators stack;

	dcl.ns = ns;
	dcl.parent = parent;
	base = specifier(&dcl.sclass, &dcl.qualifier);

	do {
		dcl.type = base;
		dcl.pars = NULL;
		stack.nr_types = stack.nr = 0;
		stack.tpars = dcl.buftpars;
		stack.pars = dcl.bufpars;
		stack.dcl = &dcl;
		stack.ns = ns;

		declarator(&stack);

		while (pop(&stack, &dcl))
			;
		sym = (*fun)(&dcl);
		if (funbody(sym, dcl.pars))
			return sym;
	} while (rep && accept(','));

	return sym;
}

void
decl(void)
{
	Symbol *sym;

	if (accept(';'))
		return;

	sym = dodcl(REP, identifier, NS_IDEN, NULL);
	if ((sym->type->prop & TFUNDEF) == 0)
		expect(';');
}

static void
fieldlist(Type *tp)
{
	if (yytoken != ';')
		dodcl(REP, field, tp->ns, tp);
	expect(';');
}

Type *
typename(void)
{
	return dodcl(NOREP, type, NS_DUMMY, NULL)->type;
}
