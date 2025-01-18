
struct objops {
	int (*type)(char *);
	int (*probe)(unsigned char *, char **);

	int (*new)(Obj *, int);
	void (*del)(Obj *);

	int (*read)(Obj *, FILE *);
	int (*write)(Obj *, Map *, FILE *);

	int (*strip)(Obj *);
	int (*pc2line)(Obj *, unsigned long long , char *, int *);

	Map *(*loadmap)(Obj *, FILE *);

	Symbol *(*getsym)(Obj *, int *, Symbol *);
	Symbol *(*setsym)(Obj *, int *, Symbol *);
	Section *(*getsec)(Obj *, int *, Section *);
	Section *(*setsec)(Obj *, int *, Section *);

	int (*setidx)(long, char *[], long[], FILE *);
	int (*getidx)(long *, char ***, long **, FILE *);
};

struct map {
	int n;
	struct mapsec {
		char *name;
		FILE *fp;
		unsigned long long begin;
		unsigned long long end;
		long offset;
	} sec[];
};

/* common functions */
extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);
extern int objpos(Obj *obj, FILE *fp, long pos);

/* globals */
extern Objops *objops[];
extern Objops coff32;
extern Objops elf64;
