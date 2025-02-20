#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/elf.h>

#include <scc/elf/elftypes.h>
#include <scc/elf/elfhdr.h>
#include <scc/elf/elfphdr.h>
#include <scc/elf/elfshdr.h>
#include <scc/elf/elfent.h>
#include <scc/elf/elfrel.h>

#include "../libmach.h"
#include "fun.h"

struct elfunpack {
	void (*hdr)(int, unsigned char *, Elfhdr *);
	void (*phdr)(int, unsigned char *, Elfphdr *);
	void (*sec)(int, unsigned char *, Elfsec *);
	void (*sym)(int, unsigned char *, Elfsym *);
	void (*rel)(int, unsigned char *, Elfrel *);
	void (*rela)(int, unsigned char *, Elfrel *);

	int hdrsiz;
	int phdrsiz;
	int shentsiz;
	int symsiz;
	int relsiz;
	int relasiz;
};

struct elfunpack unpack64;
struct elfunpack unpack32;

static void
unpack_hdr64(int order, unsigned char *buf, Elfhdr *hdr)
{
	Elf64_Ehdr ehdr;
	int n;

	n = unpack(order,
	           buf,
	           "'16sslqqqlssssss",
	           ehdr.e_ident,
	           &ehdr.e_type,
	           &ehdr.e_machine,
	           &ehdr.e_version,
	           &ehdr.e_entry,
	           &ehdr.e_phoff,
	           &ehdr.e_shoff,
	           &ehdr.e_flags,
	           &ehdr.e_ehsize,
	           &ehdr.e_phentsize,
	           &ehdr.e_phnum,
	           &ehdr.e_shentsize,
	           &ehdr.e_shnum,
	           &ehdr.e_shstrndx);

	assert(n == ELFH64SZ);

	memcpy(hdr->ident, ehdr.e_ident, EI_NIDENT);
	hdr->type = ehdr.e_type;
	hdr->machine = ehdr.e_machine;
	hdr->version = ehdr.e_version;
	hdr->entry = ehdr.e_entry;
	hdr->phoff = ehdr.e_phoff;
	hdr->shoff = ehdr.e_shoff;
	hdr->flags = ehdr.e_flags;
	hdr->ehsize = ehdr.e_ehsize;
	hdr->phentsize = ehdr.e_phentsize;
	hdr->phnum = ehdr.e_phnum;
	hdr->shentsize = ehdr.e_shentsize;
	hdr->shnum = ehdr.e_shnum;
	hdr->shstrndx = ehdr.e_shstrndx;
}

static void
unpack_hdr32(int order, unsigned char *buf, Elfhdr *hdr)
{
	Elf32_Ehdr ehdr;
	int n;

	n = unpack(order,
	           buf,
	           "'16sslllllssssss",
	           ehdr.e_ident,
	           &ehdr.e_type,
	           &ehdr.e_machine,
	           &ehdr.e_version,
	           &ehdr.e_entry,
	           &ehdr.e_phoff,
	           &ehdr.e_shoff,
	           &ehdr.e_flags,
	           &ehdr.e_ehsize,
	           &ehdr.e_phentsize,
	           &ehdr.e_phnum,
	           &ehdr.e_shentsize,
	           &ehdr.e_shnum,
	           &ehdr.e_shstrndx);

	assert(n == ELFH32SZ);

	memcpy(hdr->ident, ehdr.e_ident, EI_NIDENT);
	hdr->type = ehdr.e_type;
	hdr->machine = ehdr.e_machine;
	hdr->version = ehdr.e_version;
	hdr->entry = ehdr.e_entry;
	hdr->phoff = ehdr.e_phoff;
	hdr->shoff = ehdr.e_shoff;
	hdr->flags = ehdr.e_flags;
	hdr->ehsize = ehdr.e_ehsize;
	hdr->phentsize = ehdr.e_phentsize;
	hdr->phnum = ehdr.e_phnum;
	hdr->shentsize = ehdr.e_shentsize;
	hdr->shnum = ehdr.e_shnum;
	hdr->shstrndx = ehdr.e_shstrndx;
}

static void
unpack_phdr64(int order, unsigned char *buf, Elfphdr *hdr)
{
	int n;
	Elf64_Phdr phdr;

	n = unpack(order,
	           buf,
	           "llqqqqqq",
	           &phdr.p_type,
	           &phdr.p_flags,
	           &phdr.p_offset,
	           &phdr.p_vaddr,
	           &phdr.p_paddr,
	           &phdr.p_filesz,
	           &phdr.p_memsz,
	           &phdr.p_align);

	assert(n == ELFP64SZ);

	hdr->type = phdr.p_type;
	hdr->flags = phdr.p_flags;
	hdr->offset = phdr.p_offset;
	hdr->vaddr = phdr.p_vaddr;
	hdr->paddr = phdr.p_paddr;
	hdr->filesz = phdr.p_filesz;
	hdr->memsz = phdr.p_memsz;
	hdr->align = phdr.p_align;
}

static void
unpack_phdr32(int order, unsigned char *buf, Elfphdr *hdr)
{
	int n;
	Elf32_Phdr phdr;

	n = unpack(order,
	           buf,
	           "llllllll",
	           &phdr.p_type,
	           &phdr.p_offset,
	           &phdr.p_vaddr,
	           &phdr.p_paddr,
	           &phdr.p_filesz,
	           &phdr.p_memsz,
	           &phdr.p_flags,
	           &phdr.p_align);

	assert(n == ELFP32SZ);

	hdr->type = phdr.p_type;
	hdr->flags = phdr.p_flags;
	hdr->offset = phdr.p_offset;
	hdr->vaddr = phdr.p_vaddr;
	hdr->paddr = phdr.p_paddr;
	hdr->filesz = phdr.p_filesz;
	hdr->memsz = phdr.p_memsz;
	hdr->align = phdr.p_align;
}

static void
unpack_shdr64(int order, unsigned char *buf, Elfsec *sec)
{
	int n;
	Elf64_Shdr shdr;

	n = unpack(order,
	           buf,
	           "llqqqqllqq",
	           &shdr.sh_name,
	           &shdr.sh_type,
	           &shdr.sh_flags,
	           &shdr.sh_addr,
	           &shdr.sh_offset,
	           &shdr.sh_size,
	           &shdr.sh_link,
	           &shdr.sh_info,
	           &shdr.sh_addralign,
	           &shdr.sh_entsize);

	assert(n == ELFS64SZ);

	sec->sh_name = shdr.sh_name;
	sec->type = shdr.sh_type;
	sec->flags = shdr.sh_flags;
	sec->addr = shdr.sh_addr;
	sec->offset = shdr.sh_offset;
	sec->size = shdr.sh_size;
	sec->link = shdr.sh_link;
	sec->info = shdr.sh_info;
	sec->addralign = shdr.sh_addralign;
	sec->entsize = shdr.sh_entsize;
}

static void
unpack_shdr32(int order, unsigned char *buf, Elfsec *sec)
{
	int n;
	Elf32_Shdr shdr;

	n = unpack(order,
	           buf,
	           "llllllllll",
	           &shdr.sh_name,
	           &shdr.sh_type,
	           &shdr.sh_flags,
	           &shdr.sh_addr,
	           &shdr.sh_offset,
	           &shdr.sh_size,
	           &shdr.sh_link,
	           &shdr.sh_info,
	           &shdr.sh_addralign,
	           &shdr.sh_entsize);

	assert(n == ELFS32SZ);

	sec->sh_name = shdr.sh_name;
	sec->type = shdr.sh_type;
	sec->flags = shdr.sh_flags;
	sec->addr = shdr.sh_addr;
	sec->offset = shdr.sh_offset;
	sec->size = shdr.sh_size;
	sec->link = shdr.sh_link;
	sec->info = shdr.sh_info;
	sec->addralign = shdr.sh_addralign;
	sec->entsize = shdr.sh_entsize;
}

static void
unpack_sym64(int order, unsigned char *buf, Elfsym *sym)
{
	int n;
	Elf64_Sym ent;

	n = unpack(order,
	           buf,
	           "lccsqq",
	           &ent.st_name,
	           &ent.st_info,
	           &ent.st_other,
	           &ent.st_shndx,
	           &ent.st_value,
	           &ent.st_size);

	assert(n == ELFE64SZ);

	sym->st_name = ent.st_name;
	sym->info = ent.st_info;
	sym->other = ent.st_other;
	sym->shndx = ent.st_shndx;
	sym->value = ent.st_value;
	sym->size = ent.st_size;
}

static void
unpack_sym32(int order, unsigned char *buf, Elfsym *sym)
{
	int n;
	Elf32_Sym ent;

	n = unpack(order,
	           buf,
	           "lllccs",
	           &ent.st_name,
	           &ent.st_value,
	           &ent.st_size,
	           &ent.st_info,
	           &ent.st_other,
	           &ent.st_shndx);

	assert(n == ELFE32SZ);

	sym->st_name = ent.st_name;
	sym->info = ent.st_info;
	sym->other = ent.st_other;
	sym->shndx = ent.st_shndx;
	sym->value = ent.st_value;
	sym->size = ent.st_size;
}

static void
unpack_rel64(int order, unsigned char *buf, Elfrel *rp)
{
	int n;
	Elf64_Rel r;

	n = unpack(order,
	           buf,
	           "qq",
	           &r.r_offset,
	           &r.r_info);

	assert(n == ELFR64SZ);

	rp->off = r.r_offset;
	rp->info = r.r_info;
	rp->addend = 0;
}

static void
unpack_rel32(int order, unsigned char *buf, Elfrel *rp)
{
	int n;
	Elf32_Rel r;

	n = unpack(order,
	           buf,
	           "ll",
	           &r.r_offset,
	           &r.r_info);

	assert(n == ELFR32SZ);

	rp->off = r.r_offset;
	rp->info = r.r_info;
	rp->addend = 0;
}

static void
unpack_rela64(int order, unsigned char *buf, Elfrel *rp)
{
	int n;
	Elf64_Rela r;

	n = unpack(order,
	           buf,
	           "qqq",
	           &r.r_offset,
	           &r.r_info,
	           &r.r_addend);

	assert(n == ELFRA64SZ);

	rp->off = r.r_offset;
	rp->info = r.r_info;
	rp->addend = r.r_addend;
}

static void
unpack_rela32(int order, unsigned char *buf, Elfrel *rp)
{
	int n;
	Elf32_Rela r;

	n = unpack(order,
	           buf,
	           "lll",
	           &r.r_offset,
	           &r.r_info,
	           &r.r_addend);

	assert(n == ELFRA32SZ);

	rp->off = r.r_offset;
	rp->info = r.r_info;
	rp->addend = r.r_addend;
}

static int
readhdr(Obj *obj, FILE *fp)
{
	Elf *elf = obj->data;
	Elfhdr *hdr = &elf->hdr;
	Elfunpack *u = elf->unpack;
	unsigned char buf[ELFH64SZ];

	if (fread(buf, u->hdrsiz, 1, fp) != 1)
		return 0;
	(*u->hdr)(ORDER(obj->type), buf, hdr);

	if (hdr->shnum > INT_MAX ||  hdr->phnum > INT_MAX)
		return 0;

	switch (hdr->type) {
	case ET_REL:
	case ET_EXEC:
	case ET_DYN:
		return 1;
	default:
		return 0;
	}
}

static int
readphdr(Obj *obj, FILE *fp)
{
	int i, r;
	Elfphdr *phdr;
	Elf *elf = obj->data;
	Elfhdr *hdr = &elf->hdr;
	Elfunpack *u = elf->unpack;
	unsigned char *buf;

	r = 0;
	if (hdr->phnum > 0
	&&  (hdr->phoff == 0 || hdr->phentsize < u->phdrsiz)) {
		errno = ERANGE;
		goto err0;
	}
	if (hdr->phoff == 0 || hdr->phnum == 0)
		return 1;

	if ((buf = malloc(hdr->phentsize)) == NULL)
		goto err0;
	if ((phdr = calloc(hdr->phnum, sizeof(*phdr))) == NULL)
		goto err1;
	elf->phdr = phdr;

	if (!objpos(obj, fp, hdr->phoff))
		goto err1;

	for (i = 0; i < hdr->phnum; i++) {
		if (fread(buf, hdr->phentsize, 1, fp) != 1)
			goto err1;
		(*u->phdr)(ORDER(obj->type), buf, &phdr[i]);
		if (phdr[i].offset > LONG_MAX)
			goto  err1;
	}
	r = 1;

err1:
	free(buf);
err0:
	return r;
}

static int
readshdr(Obj *obj, FILE *fp)
{
	int r, nsec;
	Elfsec *sec;
	Elf *elf = obj->data;
	Elfhdr *hdr = &elf->hdr;
	Elfunpack *u = elf->unpack;
	unsigned char *buf;

	if (hdr->shoff == 0)
		return 1;

	if (!objpos(obj, fp, hdr->shoff))
		return 0;

	if (hdr->shnum != SHN_UNDEF) {
		if (hdr->shnum > INT_MAX)
			return 0;
		nsec = hdr->shnum;
	} else {
		Elfsec sec0;
		fpos_t pos;
		unsigned char buf0[ELFS64SZ];

		fgetpos(fp, &pos);
		fread(buf0, u->shentsiz, 1, fp);
		fsetpos(fp, &pos);

		if (ferror(fp))
			return 0;

		(*u->sec)(ORDER(obj->type), buf0, &sec0);
		if (sec0.size > INT_MAX)
			return 0;
		nsec = sec0.size;
	}

	if (nsec == 0)
		return 1;

	r = 0;
	if ((buf = malloc(hdr->shentsize)) == NULL)
		return 0;
	if ((sec = calloc(nsec, sizeof(*sec))) == NULL)
		goto err;
	elf->secs = sec;
	elf->nsec = nsec;

	for ( ; nsec-- > 0; ++sec) {
		if (fread(buf, hdr->shentsize, 1, fp) != 1)
			goto err;
		(*u->sec)(ORDER(obj->type), buf, sec);
	}
	r = 1;
err:
	free(buf);
	return r;
}

static int
readstrtbl(Obj *obj, FILE *fp)
{
	char *s;
	int idx;
	Elfsec *tbl, *sec;
	Elf *elf = obj->data;
	Elfhdr *hdr = &elf->hdr;

	if (hdr->shstrndx != SHN_XINDEX) {
		idx = hdr->shstrndx;
	} else {
		if (hdr->shnum == 0)
			return 0;
		sec = elf->secs;
		if (sec->link > INT_MAX)
			return 0;
		idx = sec->link;
	}
	if (idx > elf->nsec || elf->secs[idx].type != SHT_STRTAB)
		return 0;
	elf->secstrtbl = idx;

	for (sec = elf->secs; sec < &elf->secs[elf->nsec]; ++sec) {
		if (sec->type != SHT_STRTAB)
			continue;

		if (sec->size > SIZE_MAX)
			return 0;
		if ((s = malloc(sec->size)) == NULL)
			return 0;
		sec->strtbl = s;

		if (!objpos(obj, fp, sec->offset))
			return 0;
		if (fread(s, sec->size, 1, fp) != 1)
			return 0;
	}

	return 1;
}

static int
secsize(Elfsec *sec, int onent, int entsiz)
{
	unsigned long long nent;

	if (sec->entsize == 0 || sec->entsize < entsiz)
		return -1;
	nent = sec->size / sec->entsize;
	if (nent > INT_MAX - onent)
		return -1;

	return nent;
}

static int
readsyms(Obj *obj, Elfsec *sec, FILE *fp)
{
	int r = 0, n, oldn;
	Elfsym *sym;
	Elfsec *tbl;
	Elf *elf = obj->data;
	Elfunpack *u = elf->unpack;
	Elfhdr *hdr = &elf->hdr;
	unsigned char *buf;

	if ((n = secsize(sec, elf->nsym, u->symsiz)) <= 0)
		return n;
	if ((buf = malloc(sec->entsize)) == NULL)
		return 0;

	oldn = elf->nsym;
	sym = realloc(elf->syms, (oldn + n) * sizeof(Elfsym));
	if (!sym)
		goto err;
	elf->syms = sym;
	elf->nsym += n;

	if (!objpos(obj, fp, sec->offset))
		goto err;

	if (sec->link >= hdr->shnum)
		goto err;
	tbl = &elf->secs[sec->link];
	if (tbl->type != SHT_STRTAB)
		goto err;

	for (sym = &elf->syms[oldn] ; n-- > 0 ; ++sym) {
		if (fread(buf, sec->entsize, 1, fp) != 1)
			goto err;
		(*u->sym)(ORDER(obj->type), buf, sym);
		sym->symsec = sec;
		if (sym->st_name >= tbl->size)
			goto err;
		sym->name = &tbl->strtbl[sym->st_name];

		switch (sym->shndx) {
		case SHN_XINDEX:
			/*
			 * Elf supports an extension mechanism to allow
			 * indexes bigger than 4 bytes. We don't care
			 * and we reject elf files using this feature.
			 */
			goto err;
		case SHN_UNDEF:
		case SHN_ABS:
		case SHN_COMMON:
			break;
		default:
			if (sym->shndx >= elf->nsec)
				goto err;
			break;
		}
	}
	r = 1;

err:
	free(buf);
	return r;
}

static int
readrels(Obj *obj, Elfsec *sec, FILE *fp)
{
	int r = 0, oldn, n, min;
	Elf *elf = obj->data;
	Elfunpack *u = elf->unpack;
	Elfrel *rp;
	void (*fn)(int, unsigned char *, Elfrel *);
	unsigned char *buf;

	if (sec->type == SHT_RELA) {
		fn = u->rela;
		min = u->relasiz;
	} else {
		fn = u->rel;
		min = u->relsiz;
	}

	if (!objpos(obj, fp, sec->offset))
		goto err;

	if ((n = secsize(sec, elf->nrel, min)) <= 0)
		return n;
	if ((buf = malloc(sec->entsize)) == NULL)
		return 0;

	oldn = elf->nrel;
	rp = realloc(elf->rels, (oldn + n) * sizeof(Elfrel));
	if (!rp)
		goto err;
	elf->rels = rp;
	elf->nrel += n;

	for (rp = &elf->rels[oldn]; n-- > 0; ++rp) {
		if (fread(buf, sec->entsize, 1, fp) != 1)
			goto err;
		(*fn)(ORDER(obj->type), buf, rp);
	}
	r = 1;

err:
	free(buf);
	return r;
}

static int
procsecs(Obj *obj, FILE *fp)
{
	Elf *elf = obj->data;
	Elfsec *tbl, *sec;

	tbl = &elf->secs[elf->secstrtbl];
	for (sec = elf->secs; sec < &elf->secs[elf->nsec]; ++sec) {
		if (sec->sh_name >= tbl->size)
			return 0;
		sec->name = &tbl->strtbl[sec->sh_name];

		switch (sec->type) {
		case SHT_DYNSYM:
		case SHT_SYMTAB:
			if (!readsyms(obj, sec, fp))
				return 0;
			break;
		case SHT_RELA:
		case SHT_REL:
			if (!readrels(obj, sec, fp))
				return 0;
			break;
		}
	}

	return 1;
}

int
elfread(Obj *obj, FILE *fp)
{
	fpos_t pos;
	Elf *elf = obj->data;
	unsigned char buf[EI_CLASS+1];

	fgetpos(fp, &pos);
	fread(buf, sizeof(buf), 1, fp);
	fsetpos(fp, &pos);

	if (buf[EI_CLASS] == ELFCLASS64) {
		if (elf->is32)
			return 0;
		elf->is32 = 0;
		elf->unpack = &unpack64;
	} else {
		elf->is32 = 1;
		elf->unpack = &unpack32;
	}

	if (!readhdr(obj, fp))
		return -1;
	if (!readphdr(obj, fp))
		return -1;
	if (!readshdr(obj, fp))
		return -1;
	if (!readstrtbl(obj, fp))
		return -1;
	if (!procsecs(obj, fp))
		return -1;

	return 0;
}

struct elfunpack unpack64 = {
	.hdr = unpack_hdr64,
	.phdr = unpack_phdr64,
	.sec = unpack_shdr64,
	.sym = unpack_sym64,
	.rel = unpack_rel64,
	.rela = unpack_rela64,

	.hdrsiz = ELFH64SZ,
	.phdrsiz = ELFP64SZ,
	.shentsiz = ELFS64SZ,
	.symsiz = ELFE64SZ,
	.relsiz = ELFR64SZ,
	.relasiz = ELFRA64SZ,
};

struct elfunpack unpack32 = {
	.hdr = unpack_hdr32,
	.phdr = unpack_phdr32,
	.sec = unpack_shdr32,
	.sym = unpack_sym32,
	.rel = unpack_rel32,
	.rela = unpack_rela32,

	.hdrsiz = ELFH32SZ,
	.phdrsiz = ELFP32SZ,
	.shentsiz = ELFS32SZ,
	.symsiz = ELFE32SZ,
	.relsiz = ELFR32SZ,
	.relasiz = ELFRA32SZ,
};
