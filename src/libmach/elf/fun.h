extern int elfnew(Obj *, int);
extern int elfread(Obj *, FILE *);
extern int elfsetidx(long, char **, long *, FILE *);
extern int elfgetidx(long *, char ***, long **, FILE *);
extern int elfpc2line(Obj *, unsigned long long , char *, int *);
extern int elfstrip(Obj *);
extern void elfdel(Obj *);
extern int elfwrite(Obj *, Map *, FILE *);
extern int elfprobe(unsigned char *, char **);
extern int elftype(char *);

extern int elfxsetidx(int long , char *[], long [], FILE *);
extern int elfxgetidx(int, long *, char ***, long **, FILE *);

extern Symbol *elfgetsym(Obj *, int *, Symbol *);
extern Section *elfgetsec(Obj *, int *, Section *);
extern Map *elfloadmap(Obj *, FILE *);
