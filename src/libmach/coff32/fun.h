extern int coff32new(Obj *, int);
extern int coff32read(Obj *, FILE *);
extern int coff32setidx(long, char **, long *, FILE *);
extern int coff32getidx(long *, char ***, long **, FILE *);
extern int coff32pc2line(Obj *, unsigned long long, char *, int *);
extern int coff32strip(Obj *);
extern void coff32del(Obj *);
extern int coff32write(Obj *, Map *, FILE *);
extern int coff32probe(unsigned char *, char **);
extern int coff32type(char *);

extern int coff32xsetidx(int, long , char *[], long[], FILE *);
extern int coff32xgetidx(int, long *, char ***, long **, FILE *);

extern Symbol *coff32getsym(Obj *, int *, Symbol *);
extern Symbol *coff32setsym(Obj *, int *, Symbol *);
extern Section *coff32getsec(Obj *, int *, Section *);
extern Section *coff32setsec(Obj *, int *, Section *);
extern Map *coff32loadmap(Obj *, FILE *);
