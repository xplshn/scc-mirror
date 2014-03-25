
#ifndef SYMBOL_H
#define SYMBOL_H

#if ! __bool_true_and_false_are_defined
#include <stdbool.h>
#endif

#define CTX_OUTER 0
#define CTX_FUNC  1

enum {
	NS_IDEN = 0,
	NS_LABEL,
	NS_TAG,
	NR_NAMESPACES,
	NS_KEYWORD,
	NS_FREE
};

struct funpars;
struct symbol;

struct ctype {
	uint8_t op;           /* type builder operator */
	short size;           /* size of variables */
	short nelem;          /* number of elements in arrays */
	unsigned defined : 1; /* type defined (is not a forward reference) */
	unsigned cplex : 1;   /* complex specifier */
	unsigned imag : 1;
	unsigned sign : 1;    /* sign type */
	struct symbol *sym;   /* symbol of the tag identifier */
	struct ctype *type;   /* base type */
	struct ctype *next;   /* next element in the hash */
	union {
		struct funpar *pars;  /* function parameters */
		struct field *fields; /* aggregate fields */
	} u;
};

struct field {
	struct symbol *sym;
	struct field *next;
};

struct funpar {
	struct ctype *type;
	struct funpar *next;
};

union value {
	char c;
	int i;
	struct symbol *sym;
	uint8_t ns, token;
	short offset;
};

struct symbol {
	char *name;
	struct ctype *type;
	short id;
	uint8_t ctx;
	uint8_t token;
	uint8_t ns;
	struct {
		bool isglobal : 1;
		bool isstatic : 1;
		bool isauto : 1;
		bool isregister : 1;
	} s;
	union value u;
	struct symbol *next;
	struct symbol *hash;
};

extern void freesyms(uint8_t ns);

extern struct ctype *qualifier(struct ctype *tp, uint8_t qlf),
	*ctype(int8_t type, int8_t sign, int8_t size, int8_t cplex),
	*mktype(struct ctype *tp,
	        uint8_t op, struct symbol *tag, uint16_t nelem);

extern struct symbol
	*lookup(char *s, unsigned char ns),
	*install(char *s, unsigned char ns);

extern void context(void (*fun)(void));

extern struct ctype *voidtype,
	*uchartype,   *chartype,
	*uinttype,    *inttype,
	*ushortype,   *shortype,
	*longtype,    *ulongtype,
	*ullongtype,  *llongtype,
	*floattype,   *cfloattype,  *ifloattype,
	*doubletype,  *cdoubletype, *idoubletype,
	*ldoubletype, *cldoubletype,*ildoubletype;

#define ISQUAL(t)    (isqual((t)->op))
#define UNQUAL(t)    (ISQUAL(t) ? (t)->type : (t))
#define BTYPE(t)     (UNQUAL(t)->op)
#define isfun(op)    ((op) == FTN)
#define isptr(op)    ((op) == PTR)
#define isary(op)    ((op) == ARY)
#define isarith(op)  ((op) & ARITH)
#define isaddr(op)   ((op) & POINTER)
#define isrecord(op) ((op) & RECORD)
#define isqual(op)   ((op) & TQUALIFIER)


#endif
