#include <scc/elf/elftypes.h>
#include <scc/elf/elfhdr.h>
#include <scc/elf/elfphdr.h>
#include <scc/elf/elfshdr.h>
#include <scc/elf/elfent.h>

#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Sym Elf64_Sym
#define ELFHSZ ELFH64SZ
#define ELFPSZ ELFP64SZ
#define ELFSSZ ELFS64SZ
#define ELFESZ ELFE64SZ


#define SEC_STRTBL 0
#define SYM_STRTBL 1

typedef struct elf64 Elf64;

struct elf64 {
	Elf_Ehdr hdr;
	Elf_Phdr *phdr;
	Elf_Shdr *shdr;
	Elf_Shdr *symtab;
	Elf_Sym *syms;

	char *strtbl[2];
	size_t strsiz[2];

	size_t nsec;
	size_t nsym;
};

struct arch {
	char *name;
	int mach;
	int endian;
	int type;
};

extern char *elf64str(Obj *, int n, long);

/* globals */
extern struct arch elf64archs[];
