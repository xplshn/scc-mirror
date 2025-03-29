#define ASLABEL 0

#ifdef NDEBUG
#define PRCFG(msg)
#define PRTREE(msg)
#else
#define PRCFG(msg) (enadebug ? prcfg(msg) : NULL)
#define PRTREE(msg) (enadebug ? prforest(msg) : NULL)
#endif

enum iflags {
	BBENTRY =    1,        /* basic block entry */
	BBEXIT  =    2,        /* basic block exit */
};

enum tflags {
	SIGNF   =     1 << 0,  /* Signed type */
	INTF    =     1 << 1,  /* integer type */
	FLOATF  =     1 << 2,  /* float type */
	STRF    =     1 << 3,  /* string */
	AGGRF   =     1 << 4,  /* aggregate */
	FUNF    =     1 << 5,  /* function */
	PARF    =     1 << 6,  /* parameter */
	INITF   =     1 << 7,  /* initializer flag */
	ELLIPS  =     1 << 8,  /* vararg function */
	ARRF    =     1 << 9,  /* array flag */
	PTRF    =     1 << 10, /* pointer flag */
};

enum sclass {
	SAUTO     = 'A',
	SREG      = 'R',
	SLABEL    = 'L',
	SINDEX    = 'I',
	STMP      = 'N',
	SGLOB     = 'G',
	SEXTRN    = 'X',
	SPRIV     = 'Y',
	SLOCAL    = 'T',
	SMEMB     = 'M',
	SCONST    = '#',
	STRING    = '"',
	SNONE     = 0 /* cc2 relies on SNONE being 0 in nextpc() */
};

enum types {
	ELLIPSIS = 'E',
	INT8     = 'C',
	INT16    = 'I',
	INT32    = 'W',
	INT64    = 'Q',
	UINT8    = 'K',
	UINT16   = 'N',
	UINT32   = 'Z',
	UINT64   = 'O',
	POINTER  = 'P',
	FUNCTION = 'F',
	VECTOR   = 'V',
	UNION    = 'U',
	STRUCT   = 'S',
	BOOL     = 'B',
	FLOAT    = 'J',
	DOUBLE   = 'D',
	LDOUBLE  = 'H',
	VOID     = '0'
};

enum op {
	/* kind of operand */
	/* operands */
	OMEM     = 'M',
	OINDEX   = 'I',
	OTMP     = 'N',
	OAUTO    = 'A',
	OREG     = 'R',
	OMREG    = 'G',
	OCONST   = '#',
	OSTRING  = '"',
	OLOAD    = 'D',
	OLABEL   = 'L',
	OADD     = '+',
	OSUB     = '-',
	OMUL     = '*',
	OMOD     = '%',
	ODIV     = '/',
	OSHL     = 'l',
	OSHR     = 'r',
	OLT      = '<',
	OGT      = '>',
	OLE      = '[',
	OGE      = ']',
	OEQ      = '=',
	ONE      = '!',
	OBAND    = '&',
	OBOR     = '|',
	OBXOR    = '^',
	OCPL     = '~',
	OASSIG   = ':',
	OSNEG    = '_',
	OCALL    = 'c',
	OCALLE   = 'z',
	OPAR     = 'p',
	OFIELD   = '.',
	OCOMMA   = ',',
	OASK     = '?',
	OCOLON   = ' ',
	OADDR    = '\'',
	OAND     = 'a',
	OOR      = 'o',
	ONEG     = 'n',
	OPTR     = '@',
	OCAST    = 'g',
	OINC     = 'i',
	ODEC     = 'd',
	OBUILTIN = 'm',
	/*statements */
	ONOP     = 'q',
	OJMP     = 'j',
	OBRANCH  = 'y',
	ORET     = 'h',
	OBLOOP   = 'b',
	OELOOP   = 'e',
	OCASE    = 'v',
	ODEFAULT = 'f',
	OBSWITCH = 's',
	OESWITCH = 't',
	OBFUN    = 'x',
	OEFUN    = 'k',
};

enum builtins {
	BVA_START = 's',
	BVA_END   = 'e',
	BVA_ARG   = 'a',
	BVA_COPY  = 'c',
};

enum nerrors {
	EEOFFUN,       /* EOF while parsing function */
	ENLABEL,       /* label without statement */
	EIDOVER,       /* identifier overflow */
	EOUTPAR,       /* out pf params */
	ESYNTAX,       /* syntax error */
	ESTACKA,       /* stack unaligned */
	ESTACKO,       /* stack overflow */
	ESTACKU,       /* stack underflow */
	ELNLINE,       /* line too long */
	ELNBLNE,       /* line without new line */
	EFERROR,       /* error reading from file:%s */
	EBADID,        /* incorrect symbol id */
	EWTACKO,       /* switch stack overflow */
	EWTACKU,       /* switch stack underflow */
	ENOSWTC,       /* Out of switch statement */
	EBBUILT,       /* Unknown builtin */
	EOVERFL,       /* Numerical overflow */
	EBAFFUN,       /* Function body not finished */
	ENUMERR
};

typedef struct node Node;
typedef struct type Type;
typedef struct symbol Symbol;
typedef struct addr Addr;
typedef struct inst Inst;
typedef struct block Block;
typedef struct swtch Swtch;
typedef struct mach Mach;

struct mach {
	int swtchif;
};

struct swtch {
	int nr;
	TINT min, max;
	Node *bswtch;
	Node *eswtch;
	Node **cases;
	Node *defnode;

	Swtch *next;
};

struct type {
	unsigned long size;
	unsigned align;
	unsigned short id;
	unsigned short flags;
};

struct symbol {
	Type type;
	Type rtype;
	unsigned short id;
	unsigned short numid;
	int refcnt;
	char *name;
	char kind;
	union {
		long off;
		Node *stmt;
		Inst *inst;
	} u;
	Symbol *next, *prev;
	Symbol *h_next;
};

struct node {
	char op;
	Type type;
	int complex;
	int address;
	unsigned flags;
	union {
		TUINT i;
		TFLOAT f;
		char *s;
		Symbol *sym;
		Swtch *swtch;
		int reg;
		int subop;
		long off;
	} u;
	Symbol *label;
	Block *bb;
	Node *left, *right;
	Node *next, *prev;
};

struct block {
	int id;
	int printed, visited;
	Node *entryp, *exitp;
	Block *true, *false;
	Swtch *swtch;
	Block *next;
};

struct addr {
	char kind;
	union {
		int reg;
		TUINT i;
		Symbol *sym;
		long off;
	} u;
};

struct inst {
	unsigned char op;
	unsigned char flags;
	Symbol *label;
	Inst *next, *prev;
	Addr from1, from2, to;
};

/* main.c */
extern void error(unsigned nerror, ...);

/* parse.c */
extern void parse(void);

/* cgen.c */
extern void genaddr(void);
extern void genasm(void);
extern Node *tsethi(Node *);

/* peep.c */
extern void peephole(void);

/* code.c */
extern void data(Node *np);
extern void writeout(void), endinit(void);
extern void code(int op, Node *to, Node *from1, Node *from2);
extern void defvar(Symbol *), defpar(Symbol *), defglobal(Symbol *);
extern void setlabel(Symbol *);
extern Node *label2node(Node *np, Symbol *sym);
extern Node *constnode(Node *np, TUINT n, Type *tp);
extern Node *tmpnode(Type *, Symbol *);
extern Node *idxnode(Node *, long);
extern void delcode(void);
extern Symbol *newlabel(void);
extern void pprint(char *s);
extern void deftype(Type *);
extern Node *labelstmt(Node *, Symbol *);

/* node.c */
extern void newfun(Symbol *, Node *);
extern void apply(Node *(*fun)(Node *));
extern void cleannodes(void);
extern void delnode(Node *np);
extern void deltree(Node *np);
extern void prtree(Node *np), prforest(char *msg);
extern Node *node(int op);
extern Node *addstmt(Node *);
extern Node *delstmt(Node *);
extern Node *insstmt(Node *, Node *);
extern Node *prestmt(Node *);
extern void delrange(Node *, Node *);
extern Node *unlinkstmt(Node *);

/* symbol.c */
#define TMPSYM  0
extern Symbol *getsym(unsigned id);
extern void popctx(void);
extern void pushctx(void);
extern void freesym(Symbol *sym);

/* cfg.c */
extern void gencfg(void);
extern void cleancfg(void);
extern Node *sethi(Node *);

/* swtch.c */
extern void cleanswtch(void);
extern Swtch *newswtch(Swtch *);
extern Node *swtch(Node *);
extern Node *swtchdefault(Swtch *);

/* globals */
extern Symbol *curfun;
extern Symbol *locals;
extern Inst *pc, *prog;
extern Node *laststmt;
extern Mach mach;

/* target */
extern Type int8type, int16type, int32type, int64type,
            uint8type, uint16type, uint32type, uint64type,
            float32type, float64type, float80type,
            booltype,
            ptrtype,
            voidtype,
            arg_type;
