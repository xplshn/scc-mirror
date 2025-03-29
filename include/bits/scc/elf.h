#define EI_NIDENT               16

typedef struct elf Elf;
typedef struct elfhdr Elfhdr;
typedef struct elfphdr Elfphdr;
typedef struct elfsec Elfsec;
typedef struct elfsym Elfsym;
typedef struct elfpack Elfpack;
typedef struct elfunpack Elfunpack;
typedef struct elfrel Elfrel;

struct elfhdr {
	unsigned char ident[EI_NIDENT];
	unsigned short type;
	unsigned short machine;
	unsigned long version;
	unsigned long long entry;
	unsigned long long phoff;
	unsigned long long shoff;
	unsigned long flags;
	unsigned short ehsize;

	unsigned short phentsize;
	unsigned short phnum;

	unsigned short shentsize;
	unsigned short shnum;
	unsigned short shstrndx;
};

struct elfphdr {
	unsigned long type;
	unsigned long flags;
	unsigned long long offset;

	unsigned long long vaddr;
	unsigned long long paddr;

	unsigned long long filesz;
	unsigned long long memsz;
	unsigned long long align;
};

struct elfsec {
	char *name;
	char *strtbl;

	unsigned long sh_name;
	unsigned long type;
	unsigned long long flags;
	unsigned long long addr;
	unsigned long long offset;
	unsigned long long size;
	unsigned long link;
	unsigned long info;
	unsigned long long addralign;
	unsigned long long entsize;
};

struct elfrel {
	Elfsym *sym;
	Elfsec *sec;

	unsigned long long off;
	unsigned long info;
	long addend;
};

struct elfsym {
	char *name;
	Elfsec *symsec;

	unsigned long st_name;
	unsigned char info;
	unsigned char other;
	unsigned short shndx;
	unsigned long long value;
	unsigned long long size;
};

struct elf {
	Elfhdr hdr;
	Elfphdr *phdr;
	int is32;

	Elfsec *secs;
	int nsec;
	int secstrtbl;

	Elfsym *syms;
	int nsym;

	Elfrel *rels;
	int nrel;

	Elfpack *pack;
	Elfunpack *unpack;
};

struct arch {
	char *name;
	int mach;
	int endian;
	int type;
	int is32;
};

/* globals */
extern struct arch elfarchs[];
