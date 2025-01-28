typedef struct flags Flags;

enum file_flags {
	HAS_RELOC,
	EXEC_P,
	HAS_LINENO,
	HAS_DEBUG,
	HAS_SYMS,
	HAS_LOCALS,
	DYNAMIC,
	NR_FILE_FLAGS,
};

enum sec_flags {
	SEC_HAS_CONTENTS,
	SEC_ALLOC,
	SEC_LOAD,
	SEC_RELOC,
	SEC_READONLY,
	SEC_CODE,
	SEC_DATA,
	SEC_DEBUGGING,
	NR_SEC_FLAGS,
};

struct flags {
	int nr;
	unsigned long flags;
	char *text[];
};

/* coff32.c */
extern void coff32syms(Obj *);
extern void coff32scns(Obj *);
extern void coff32fhdr(Obj *, unsigned long long *, Flags *);
extern int coff32hasrelloc(Obj *, Section *);

/* elf64.c */
extern void elfsyms(Obj *);
extern void elfscns(Obj *);
extern void elffhdr(Obj *, unsigned long long *, Flags *);
extern int elfhasrelloc(Obj *, Section *);

/* main.c */
extern void error(char *, ...);
extern void setflag(Flags *, int, int);
extern void printflags(Flags *);
extern int selected(char *);

/* globals */
extern int pflag;
