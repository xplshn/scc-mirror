extern int elf64new(Obj *, int);
extern int elf64read(Obj *, FILE *);
extern int elf64setidx(long, char **, long *, FILE *);
extern int elf64getidx(long *, char ***, long **, FILE *);
extern int elf64pc2line(Obj *, unsigned long long , char *, int *);
extern int elf64strip(Obj *);
extern void elf64del(Obj *);
extern int elf64write(Obj *, Map *, FILE *);
extern int elf64probe(unsigned char *, char **);
extern int elf64type(char *);

extern int elf64xsetidx(int long , char *[], long [], FILE *);
extern int elf64xgetidx(int, long *, char ***, long **, FILE *);

extern Symbol *elf64getsym(Obj *, int *, Symbol *);
extern Section *elf64getsec(Obj *, int *, Section *);
extern Map *elf64loadmap(Obj *, FILE *);
