#ifdef stdin
extern int copysec(Mapsec *, FILE *);
#endif

/* common functions */
extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);
