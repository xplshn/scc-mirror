struct mapsec {
	char *name;
	FILE *fp;
	unsigned long long begin;
	unsigned long long end;
	long fsiz;
	long offset;
};

struct map {
	int nsec, nseg;
	struct mapsec *sec;
	struct mapsec *seg;
};

/* common functions */
extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);
