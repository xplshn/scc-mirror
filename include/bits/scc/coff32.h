#include <scc/coff32/filehdr.h>
#include <scc/coff32/aouthdr.h>
#include <scc/coff32/scnhdr.h>
#include <scc/coff32/syms.h>
#include <scc/coff32/reloc.h>
#include <scc/coff32/linenum.h>
#include <scc/coff32/storclass.h>

enum {
	SYM_ENT,
	SYM_AUX_UNK,
	SYM_AUX_SYM,
	SYM_AUX_FILE,
	SYM_AUX_SCN,
	SYM_AUX_FUN,
	SYM_AUX_ARY,
};

typedef struct coff32 Coff32;
typedef struct entry Entry;

struct entry {
	char type;
	union {
		SYMENT sym;
		AUXENT aux;
		unsigned char buf[AUXESZ];
	} u;
};

struct coff32 {
	FILHDR hdr;
	AOUTHDR aout;
	SCNHDR *scns;
	Entry *ents;
	RELOC **rels;
	LINENO **lines;
	char *strtbl;
	unsigned long strsiz;
};

struct arch {
	char *name;
	unsigned char magic[2];
	int type;
	int flags;
};

extern char *coff32str(Coff32 *, void *);


/* globals */
extern struct arch coff32archs[];
