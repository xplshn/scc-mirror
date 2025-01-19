#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <scc/ar.h>
#include <scc/arg.h>
#include <scc/scc.h>
#include <scc/mach.h>

#include "objdump.h"

int tflag, fflag, hflag, pflag, aflag, rflag;
char *argv0;

static int status, nsecs;
static char *filename, *membname, **secs;

void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "objdump: %s: ", filename);
	if (membname)
		fprintf(stderr, "%s: ", membname);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);

	status = EXIT_FAILURE;
}

void
setflag(unsigned *r, int cond, int flag)
{
	if (cond)
		*r |= 1 << flag;
}

static void
printfileflags(unsigned flags)
{
	int first, i;
	static const char *text[] = {
		[HAS_RELOC] = "HAS_RELOC",
		[EXEC_P] = "EXEC_P",
		[HAS_LINENO] = "HAS_LINENO",
		[HAS_DEBUG] = "HAS_DEBUG",
		[HAS_SYMS] = "HAS_SYMS",
		[HAS_LOCALS] = "HAS_LOCALS",
		[D_PAGED] = "D_PAGED",
	};

	first = 1;
	for (i = 0; i < NR_FILE_FLAGS; i++) {
		if (flags & 1) {
			if (!first)
				fputs(", ", stdout);
			first = 0;
			fputs(text[i], stdout);
		}
		flags >>= 1;
	}

	putchar('\n');
}

static void
dumpfhdr(Obj *obj, char *fmt)
{
	unsigned f;
	unsigned long long start = 0;

	printf("architecture: %s, flags: 0x%08x\n",
	       strchr(fmt, '-') + 1,
	       obj->type);

	switch (FORMAT(obj->type)) {
	case COFF32:
		f = coff32fhdr(obj, &start);
		break;
	default:
		error("unknown fhdr binary format");
		return;
	}

	printfileflags(f);
	printf("start address 0x%08llx\n", start);
}

static void
printsecflags(unsigned flags)
{
	int first, i;
	static const char *text[] = {
		[SEC_HAS_CONTENTS] = "CONTENTS",
		[SEC_ALLOC] = "ALLOC",
		[SEC_LOAD] = "LOAD",
		[SEC_RELOC] = "RELOC",
		[SEC_READONLY] = "READONLY",
		[SEC_CODE] = "CODE",
		[SEC_DATA] = "DATA",
		[SEC_DEBUGGING] = "DEBUGGING",
	};

	first = 1;
	fputs("                  ", stdout);
	for (i = 0; i < NR_SEC_FLAGS; i++) {
		if (flags & 1) {
			if (!first)
				fputs(", ", stdout);
			first = 0;
			fputs(text[i], stdout);
		}
		flags >>= 1;
	}

	putchar('\n');
}

static int
logb2(unsigned val)
{
	int n;

	for (n = 0; (val & 1) == 0; n++)
		val >>= 1;
	return n;
}

int
selected(char *secname)
{
	int i;

	if (nsecs == 0)
		return 1;

	for (i = 0; i < nsecs; i++) {
		if (strcmp(secname, secs[i]) == 0)
			return 1;
	}

	return 0;
}

static void
dumpscns(Obj *obj)
{
	int i, debug;
	unsigned f, flags;
	Section sec;

	puts("Sections:");
	puts("Idx Name          Size      VMA       LMA       File off  Algn");
	for (i = 0; getsec(obj, &i, &sec); i++) {
		if (!selected(sec.name))
			continue;

		printf("%3d %-13s %08llx  %08llx  %08llx  %08llx  2**%d\n",
		       sec.index,
		       sec.name,
		       sec.size,
		       sec.base,
		       sec.load,
		       sec.offset,
		       logb2(sec.align));

		f = 0;
		flags = sec.flags;
		debug = sec.type == 'N';
		setflag(&f, flags & SALLOC, SEC_ALLOC);
		setflag(&f, flags & SLOAD, SEC_LOAD);
		setflag(&f, (flags & SRELOC) && sec.nreloc > 0, SEC_RELOC);
		setflag(&f, (flags & SWRITE) == 0 && !debug, SEC_READONLY);
		setflag(&f, flags & SEXEC, SEC_CODE);
		setflag(&f, (flags & (SEXEC|SLOAD)) == SLOAD, SEC_DATA);
		setflag(&f, debug, SEC_DEBUGGING);
		setflag(&f, sec.size > 0, SEC_HAS_CONTENTS);
		printsecflags(f);
	}

	if (!pflag)
		return;

	switch (FORMAT(obj->type)) {
	case COFF32:
		coff32scns(obj);
		break;
	default:
		error("unknown fhdr binary format");
	}
}

static void
dumpsyms(Obj *obj)
{
	puts("SYMBOL TABLE:");
	switch (FORMAT(obj->type)) {
	case COFF32:
		coff32syms(obj);
		break;
	default:
		error("unknown symbol binary format");
	}
}

static void
dumpobj(FILE *fp, int type, char *fmt)
{
	Obj *obj;

	printf("\n%s", filename);
	if (membname)
		printf("(%s)", membname);
	printf(":\tfile format %s\n", fmt);

	if ((obj = newobj(type)) == NULL) {
		error("failed object allocation");
		return;
	}

	if (readobj(obj, fp) < 0) {
		error("object file corrupted");
		goto err;
	}

	if (fflag)
		dumpfhdr(obj, fmt);
	if (hflag)
		dumpscns(obj);
	if (tflag)
		dumpsyms(obj);

err:
	delobj(obj);
}

static void
dumprights(unsigned r)
{
	putchar((r & 4) ? 'r' : '-');
	putchar((r & 2) ? 'w' : '-');
	putchar((r & 1) ? 'x' : '-');
}

static void
dumpar(char *fname, struct ar_hdr *hdr, char *fmt)
{
	time_t t;
	int n;
	struct tm *tm;
	char buf[60];
	unsigned long mode;

	printf("%s:     file format %s\n", fname, fmt);

	mode = strtol(hdr->ar_mode, NULL, 8);
	dumprights((mode >> 6) & 7);
	dumprights((mode >> 3) & 7);
	dumprights(mode & 7);

	t = fromepoch(atoll(hdr->ar_date));
	strftime(buf, sizeof(buf), "%c", gmtime(&t));
	printf(" %d/%d   %lld %s %s\n\n",
	       atoi(hdr->ar_uid),
	       atoi(hdr->ar_gid),
	       atoll(hdr->ar_size),
	       buf,
	       fname);
}

static void
dumplib(FILE *fp)
{
	int t;
	char *fmt;
	long off, cur;
	struct ar_hdr hdr;
	char memb[SARNAM+1];

	for (;;) {
		cur = ftell(fp);

		off = armember(fp, memb, &hdr);
		switch (off) {
		case -1:
			error("library corrupted");
			if (ferror(fp))
				error(strerror(errno));
		case 0:
			return;
		default:
			membname = memb;
			if ((t = objprobe(fp, &fmt)) != -1) {
				if (aflag)
					dumpar(memb, &hdr, fmt);
				dumpobj(fp, t, fmt);
			}
			membname = NULL;
			fseek(fp, cur, SEEK_SET);
			fseek(fp, off, SEEK_CUR);
			break;
		}
	}
}

static void
objdump(char *fname)
{
	int t;
	char *fmt;
	FILE *fp;

	membname = NULL;
	filename = fname;
	if ((fp = fopen(fname, "rb")) == NULL) {
		error(strerror(errno));
		return;
	}

	if ((t = objprobe(fp, &fmt)) != -1)
		dumpobj(fp, t, fmt);
	else if (archive(fp))
		dumplib(fp);
	else
		error("bad format");
			
	fclose(fp);
}

static void
usage(void)
{
	fputs("usage: objdump [-afhpt][-j section] file...\n", stderr);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	char *s;

	ARGBEGIN {
	case 'a':
		aflag = 1;
		break;
	case 'f':
		fflag = 1;
		break;
	case 'h':
		hflag = 1;
		break;
	case 'p':
		pflag = 1;
		break;
	case 't':
		tflag = 1;
		break;
	case 'j':
		s = EARGF(usage());
		secs = xrealloc(secs, (nsecs + 1) * sizeof(char *));
		secs[nsecs++] = s;
		break;		
	default:
		usage();
	} ARGEND

	if (!aflag && !fflag && !hflag && !tflag) {
		fputs("objdump: At lest one of [afht] flags must be used\n",
		      stderr);
		usage();
	}

	if (argc == 0) {
		objdump("a.out");
	} else {
		for ( ; *argv; ++argv)
			objdump(*argv);
	}

	if (fflush(stdout)) {
		fprintf(stderr,
		        "size: error writing in output:%s\n",
		        strerror(errno));
		status = 1;
	}

	return status;
}
