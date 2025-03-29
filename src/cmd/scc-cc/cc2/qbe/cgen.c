#include <assert.h>
#include <stdlib.h>

#include <scc/cstd.h>
#include <scc/scc.h>

#include "../cc2.h"
#include "arch.h"

#define I1BYTES 0
#define I2BYTES 1
#define I4BYTES 2
#define I8BYTES 3

static Node *cgen(Node *);

static unsigned char opasmw[][2] = {
	[OADD] = {ASADDW, ASADDW},
	[OSUB] = {ASSUBW, ASSUBW},
	[OMUL] = {ASMULW, ASMULW},
	[OMOD] = {ASMODW, ASUMODW},
	[ODIV] = {ASDIVW, ASUDIVW},
	[OSHL] = {ASSHLW, ASSHLW},
	[OSHR] = {ASSHRW, ASUSHRW},
	[OLT] = {ASLTW, ASULTW},
	[OGT] = {ASGTW, ASUGTW},
	[OLE] = {ASLEW, ASULEW},
	[OGE] = {ASGEW, ASUGEW},
	[OEQ] = {ASEQW, ASEQW},
	[ONE] = {ASNEW, ASNEW},
	[OBAND] = {ASBANDW, ASBANDW},
	[OBOR] = {ASBORW, ASBORW},
	[OBXOR] = {ASBXORW, ASBXORW},
	[OSNEG] = {ASNEGW, ASNEGW},
};

static unsigned char opasml[][2] = {
	[OADD] = {ASADDL, ASADDL},
	[OSUB] = {ASSUBL, ASSUBL},
	[OMUL] = {ASMULL, ASMULL},
	[OMOD] = {ASMODL, ASUMODL},
	[ODIV] = {ASDIVL, ASUDIVL},
	[OSHL] = {ASSHLL, ASSHLL},
	[OSHR] = {ASSHRL, ASUSHRL},
	[OLT] = {ASLTL, ASULTL},
	[OGT] = {ASGTL, ASUGTL},
	[OLE] = {ASLEL, ASULEL},
	[OGE] = {ASGEL, ASUGEL},
	[OEQ] = {ASEQL, ASEQL},
	[ONE] = {ASNEL, ASNEL},
	[OBAND] = {ASBANDL, ASBANDL},
	[OBOR] = {ASBORL, ASBORL},
	[OBXOR] = {ASBXORL, ASBXORL},
	[OSNEG] = {ASNEGL, ASNEGL},
};

static unsigned char opasms[][2] = {
	[OADD] = {ASADDS, ASADDS},
	[OSUB] = {ASSUBS, ASSUBS},
	[OMUL] = {ASMULS, ASMULS},
	[ODIV] = {ASDIVS, ASDIVS},
	[OLT] = {ASLTS, ASLTS},
	[OGT] = {ASGTS, ASGTS},
	[OLE] = {ASLES, ASLES},
	[OGE] = {ASGES, ASGES},
	[OEQ] = {ASEQS, ASEQS},
	[ONE] = {ASNES, ASNES},
	[OSNEG] = {ASNEGS, ASNEGS},
};

static unsigned char opasmd[][2] = {
	[OADD] = {ASADDD, ASADDD},
	[OSUB] = {ASSUBD, ASSUBD},
	[OMUL] = {ASMULD, ASMULD},
	[ODIV] = {ASDIVD, ASDIVD},
	[OLT] = {ASLTD, ASLTD},
	[OGT] = {ASGTD, ASGTD},
	[OLE] = {ASLED, ASLED},
	[OGE] = {ASGED, ASGED},
	[OEQ] = {ASEQD, ASEQD},
	[ONE] = {ASNED, ASNED},
	[OSNEG] = {ASNEGD, ASNEGD},
};

static unsigned char (*opbin[][2])[2] = {
	{opasmw, opasml},
	{opasms, opasmd},
};

static unsigned char i2i_conv[4][4][2] = {
	[I1BYTES] = {
		[I4BYTES] = {ASEXTBW, ASUEXTBW},
		[I8BYTES] = {ASEXTBL, ASUEXTBL},
	},
	[I2BYTES] = {
		[I4BYTES] = {ASEXTHW, ASUEXTHW},
		[I8BYTES] = {ASEXTHL, ASUEXTHL},
	},
	[I4BYTES] = {
		[I8BYTES] = {ASEXTWL, ASUEXTWL},
	}
};

static unsigned char f2i_conv[4][4][2] = {
	[I4BYTES] = {
		[I4BYTES] = {ASSTOW, ASSTOUW},
		[I8BYTES] = {ASSTOL, ASDTOUL},
	},
	[I8BYTES] = {
		[I4BYTES] = {ASDTOW, ASDTOUW},
		[I8BYTES] = {ASDTOL, ASDTOUL},
	}
};

static unsigned char i2f_conv[4][4][2] = {
	[I4BYTES] = {
		[I4BYTES] = {ASSWTOS, ASUWTOS},
		[I8BYTES] = {ASSWTOD, ASUWTOD},
	},
	[I8BYTES] = {
		[I4BYTES] = {ASSLTOS, ASULTOS},
		[I8BYTES] = {ASSLTOD, ASULTOD},
	}
};

/*
 * This is strongly influenced by
 * http://plan9.bell-labs.com/sys/doc/compiler.ps (/sys/doc/compiler.ps)
 * calculate addresability as follows
 *     AUTO => 11          value+fp
 *     REG => 11           reg
 *     STATIC => 11        (value)
 *     CONST => 11         $value
 * These values of addressability are not used in the code generation.
 * They are only used to calculate the Sethi-Ullman numbers. Since
 * QBE is AMD64 targered we could do a better job there, and try to
 * detect some of the complex addressing modes of these processors.
 */
Node *
tsethi(Node *np)
{
	int op;
	Node *l, *r;

	l = np->left;
	r = np->right;

	switch (np->op) {
	case OAUTO:
	case OREG:
	case OMEM:
	case OCONST:
		np->address = 11;
		break;
	case OASSIG:
		assert(l->op != OCAST);
		goto binary;
	case OCPL:
		assert(np->type.flags & INTF);
		np->op = OBXOR;
		r = constnode(NULL, ~(TUINT) 0, &np->type);
		goto binary;
	case OEFUN:
		/*
		 * In QBE we need at the end of a basic block
		 * a jump, so we have to ensure that the last
		 * statement of the function is a ret, a jmp
		 * or a branch.
		 */

		op = np->prev->op;
		if (op == ONOP || op == OBRANCH || (op != ORET && op != OJMP))
			addstmt(node(ORET));
		break;
	default:
	binary:
		l = sethi(l);
		r = sethi(r);
		break;
	}
	np->left = l;
	np->right = r;

	return np;
}

static int
bytes2idx(int nbytes)
{
	if (nbytes== 1)
		return I1BYTES;
	else if (nbytes == 2)
		return I2BYTES;
	else if (nbytes == 4)
		return I4BYTES;
	else if (nbytes == 8)
		return I8BYTES;
	else
		abort();
}

static Node *
load(Type *tp, Node *np)
{
	int op;
	Node *new;
	int flags = tp->flags;

	if (flags & (AGGRF|FUNF|ARRF|PTRF))
		return np;

	switch (tp->size) {
	case 1:
		op = ASLDSB;
		break;
	case 2:
		op = ASLDSH;
		break;
	case 4:
		op = (flags & FLOATF) ? ASLDS : ASLDSW;
		break;
	case 8:
		op = (flags & FLOATF) ? ASLDD : ASLDL;
		break;
	default:
		abort();
	}
	/*
	 * unsigned version of operations are always +1 the
	 * signed version
	 */
	if ((flags & (INTF|SIGNF)) == INTF && tp->size < 8)
		++op;

	new = tmpnode(tp, NULL);
	code(op, new, np, NULL);

	return new;
}

static Node *rhs(Node *np);

static Node *
cast(Type *td, Node *np)
{
	Type *ts;
	Node *tmp;
	int op, d_isint, s_isint, sidx, didx, sign;

	ts = &np->type;
	d_isint = (td->flags & INTF) != 0;
	s_isint = (ts->flags & INTF) != 0;

	sidx = bytes2idx(ts->size);
	didx = bytes2idx(td->size);

	sign = (ts->flags & SIGNF) == 0;

	if (d_isint && s_isint) {
		/* conversion from int to int */
		if (didx < I4BYTES)
			didx = bytes2idx(4);

		if (didx <= sidx) {
			np->type = *td;
			return np;
		}

		op = i2i_conv[sidx][didx][sign];
	} else if (d_isint) {
		/* conversion from float to int */
		if (didx < I4BYTES)
			didx = bytes2idx(4);

		op = f2i_conv[sidx][didx][sign];
	} else if (s_isint) {
		/* conversion from int to float */
		if (sidx == I1BYTES || sidx == I2BYTES) {
			ts = (ts->flags&SIGNF) ? &int32type : &uint32type;
			np = cast(ts, np);
		}
		op = i2f_conv[sidx][didx][sign];
	} else {
		/* conversion from float to float */
		op = (didx == I4BYTES) ? ASEXTS : ASTRUNCD;
	}

	assert(op != 0);
	tmp = tmpnode(td, NULL);
	code(op, tmp, np, NULL);

	return tmp;
}

static Node *
call(Node *np, Node *fun)
{
	int n, op;
	Type *tp;
	Node **q, *tmp, *p, *pars[NR_FUNPARAM];

	for (n = 0, p = np->right; p; p = p->right)
		pars[n++] = rhs(p->left);

	tp = &np->type;
	tmp = tmpnode(tp, NULL);
	code(ASCALL, tmp, fun, NULL);

	for (q = pars; q < &pars[n]; ++q) {
		op = (q == &pars[n-1]) ? ASPARE : ASPAR;
		code(op, NULL, *q, tmpnode(&(*q)->type, NULL));
	}
	code((np->op == OCALL) ? ASCALLE : ASCALLEX, NULL, NULL, NULL);

	return tmp;
}

static Node *
copy(Type *tp, Node *to, Node *from)
{
	int op;

	switch (tp->size) {
	case 0:
		return from;
	case 1:
		op = ASCOPYB;
		break;
	case 2:
		op = ASCOPYH;
		break;
	case 4:
		op = (tp->flags & FLOATF) ? ASCOPYS : ASCOPYW;
		break;
	case 8:
		op = (tp->flags & FLOATF) ? ASCOPYD : ASCOPYL;
		break;
	default:
		abort();
	}
	code(op, to, from, NULL);
	return from;
}

static Node *
field(Node *np, int islhs)
{
	Node *tmp, *addr;
	TUINT offset = np->right->u.sym->u.off;

	addr = rhs(np->left);
	tmp = node(OADD);
	tmp->type = ptrtype;
	tmp->left = addr;
	tmp->right = constnode(NULL, offset, &ptrtype);
	addr = rhs(tmp);

	if (!islhs)
		addr = load(&np->type, addr);
	return addr;
}

static Node *
lhs(Node *np)
{
	switch (np->op) {
	case OREG:
	case OMEM:
	case OAUTO:
		return np;
	case OPTR:
		return rhs(np->left);
	case OFIELD:
		return field(np, 1);
	default:
		abort();
	}
}

static Node *
function(void)
{
	Node aux;
	Symbol *p;

	/* allocate stack space for parameters */
	for (p = locals; p; p = p->next) {
		if ((p->type.flags & PARF) == 0)
			continue;
		if ((p->type.flags & AGGRF) != 0)
			continue;
		code(ASALLOC, label2node(&aux, p), NULL, NULL);
	}

	/* allocate stack space for local variables) */
	for (p = locals; p; p = p->next) {
		if ((p->type.flags & PARF) != 0)
			continue;
		if (p->kind != SAUTO || p->id == TMPSYM)
			continue;
		code(ASALLOC, label2node(&aux, p), NULL, NULL);
	}

	/* store formal parameters in parameters */
	for (p = locals; p; p = p->next) {
		if ((p->type.flags & PARF) == 0)
			continue;
		if ((p->type.flags & AGGRF) != 0)
			continue;
		if ((p->type.flags & (ARRF|AGGRF)) == 0)
			code(ASFORM, label2node(&aux, p), NULL, NULL);
	}
	return NULL;
}

static int
assignop(Type *tp)
{
	int flags = tp->flags;

	if (flags & (AGGRF|FUNF|ARRF))
		return ASSTM;

	switch (tp->size) {
	case 1:
		return ASSTB;
	case 2:
		return ASSTH;
	case 4:
		return (tp->flags & FLOATF) ? ASSTS : ASSTW;
	case 8:
		return (tp->flags & FLOATF) ? ASSTD : ASSTL;
	default:
		abort();
	}
}

static void
rhs_rhs(Node **lpp, Node **rpp)
{
	Node *l = *lpp, *r = *rpp;

	if (l->complex >= r->complex) {
		l = rhs(l);
		r = rhs(r);
	} else {
		r = rhs(r);
		l = rhs(l);
	}

	*lpp = l, *rpp = r;
}

static void
lhs_rhs(Node **lpp, Node **rpp)
{
	Node *l = *lpp, *r = *rpp;

	if (l->complex >= r->complex) {
		l = lhs(l);
		r = rhs(r);
	} else {
		r = rhs(r);
		l = lhs(l);
	}

	*lpp = l, *rpp = r;
}

static Node *
assign(Node *np)
{
	Node *ret, aux;
	Node *l = np->left, *r = np->right;
	int op;

	switch (np->u.subop) {
	case OINC:
		op = OADD;
		goto post_oper;
	case ODEC:
		op = OSUB;
	post_oper:
		lhs_rhs(&l, &r);
		ret = load(&l->type, l);
		aux.op = op;
		aux.left = ret;
		aux.right = r;
		aux.type = np->type;
		r = rhs(sethi(&aux));
		break;
	default:
		/* assign abbreviation */
		assert(l->type.size == r->type.size);
		if (r->type.size < 4) {
			lhs_rhs(&l, &r);
			aux.op = np->u.subop;
			aux.left = load(&r->type, l);
			aux.right = r;
			aux.type = int32type;
			aux.address = np->address;
			ret = r = sethi(rhs(&aux));
			break;
		}

		aux.op = np->u.subop;
		aux.left = l;
		aux.right = r;
		aux.type = np->type;
		aux.address = np->address;
		r = sethi(&aux);
	case 0:
		if (l->op == OTMP) {
			r = rhs(r);
			copy(&np->type, l, r);
			return r;
		}

		lhs_rhs(&l, &r);
		ret = r;
		break;
	}

	code(assignop(&np->type), l, r, NULL);
	return ret;
}

static Node *
rhs(Node *np)
{
	Node *tmp, aux1, aux2;
	Node *phi, *l = np->left, *r = np->right;
	Type *tp;
	int sign, size, isfloat, op;
	Symbol *true, *false;

	tp = &np->type;

	switch (np->op) {
	case OTMP:
	case OCONST:
		return np;
	case OMEM:
	case OREG:
	case OAUTO:
		return load(tp, np);
	case OMOD:
	case OSHL:
	case OBAND:
	case OBOR:
	case OBXOR:
	case OSHR:
		assert(tp->flags & INTF);
	case ODIV:
	case OLT:
	case OGT:
	case OLE:
	case OGE:
	case OADD:
	case OSUB:
	case OMUL:
	case OEQ:
	case ONE:
		assert(tp->size == 4 || tp->size == 8);

		sign = (tp->flags & SIGNF) == 0;
		size = tp->size == 8;
		isfloat = (tp->flags & FLOATF) != 0;
		op = opbin[isfloat][size][np->op][sign];
		rhs_rhs(&l, &r);
		tmp = tmpnode(tp, NULL);
		code(op, tmp, l, r);
		return tmp;
	case OCALL:
	case OCALLE:
		if (l->op == OPTR)
			l = rhs(l);
		return call(np, l);
	case OCAST:
		return cast(tp, rhs(l));
	case OASSIG:
		return assign(np);
	case OSNEG:
		sign = (tp->flags & SIGNF) == 0;
		size = tp->size == 8;
		isfloat = (tp->flags & FLOATF) != 0;
		op = opbin[isfloat][size][np->op][sign];
		tmp = tmpnode(tp, NULL);
		code(op, tmp, rhs(l), NULL);
		return tmp;
	case OPTR:
		return load(tp, rhs(l));
	case OADDR:
		l = lhs(l);
		l->type = *tp;
		l->type.flags |= PTRF;
		return l;
	case OFIELD:
		return field(np, 0);
	case OBUILTIN:
		switch (np->u.subop) {
		case BVA_START:
			l = rhs(l);
			code(ASVSTAR, NULL, l, NULL);
		case BVA_END:
			return NULL;
		case BVA_ARG:
			l = rhs(l);
			tmp = tmpnode(tp, NULL);
			code(ASVARG, tmp, l, NULL);
			return tmp;
		default:
			abort();
		}
	default:
		abort();
	}
	abort();
}

static Node *
cgen(Node *np)
{
	Node true, false, *p, *next;

	setlabel(np->label);
	switch (np->op) {
	case OBFUN:
		return function();
	case ONOP:
	case OBLOOP:
	case OELOOP:
	case OEFUN:
		break;
	case OJMP:
		label2node(&true, np->u.sym);
		code(ASJMP, NULL, &true, NULL);
		break;
	case OBRANCH:
		/*
		 * QBE does not accept labels at the end of a function
		 * (ONOP is used for labels) so we have to add a ret
		 * there, and in the case of branches we need a label
		 * for the next statement.
		 */
		next = np->next;
		if (!next->label)
			labelstmt(next, NULL);

		label2node(&true, np->u.sym);
		label2node(&false, np->next->label);
		code(ASBRANCH, rhs(np->left), &true, &false);
		break;
	case ORET:
		p = (np->left) ? rhs(np->left) : NULL;
		code(ASRET, NULL, p, NULL);
		break;
	default:
		rhs(np);
		break;
	}
	return NULL;
}

void
genasm(void)
{
	apply(cgen);
}
