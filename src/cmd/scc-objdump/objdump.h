enum file_flags {
	HAS_RELOC,
	EXEC_P,
	HAS_LINENO,
	HAS_DEBUG,
	HAS_SYMS,
	HAS_LOCALS,
	D_PAGED,
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

/* coff32.c */
extern void coff32syms(Obj *);
extern void coff32scns(Obj *);
extern unsigned coff32fhdr(Obj *, unsigned long long *);

/* main.c */
extern void error(char *, ...);
extern void setflag(unsigned *, int, int);
extern int selected(char *);

/* globals */
extern int pflag;
