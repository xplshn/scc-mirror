/*
 * This file is inspired in:
 *	- the book "Understanding and using COFF"
 *	- the book "UNIX System V 386 R3.2 Programmers guide, Vol 2"
 */

#define SYMNMLEN   8

#define SYMENT  struct syment
#define SYMESZ  18

#define AUXENT  union auxent
#define AUXESZ  18

#define n_name       _n._n_name
#define n_zeroes     _n._n_n._n_zeroes
#define n_offset     _n._n_n._n_offset
#define n_nptr       _n._n_nptr[1]

#define FILNMLEN  14
#define EFILNMLEN 20

struct syment {
	union {
		char _n_name[SYMNMLEN];  /* symbol name */
		struct {
			long _n_zeroes;  /* if _n_name[0-3] == 0 */
			long _n_offset;  /* offset into string table */
		} _n_n;
		char *_n_nptr[2];        /* allows overlaying */
	} _n;
	unsigned long n_value;           /* value of symbol */
	short n_scnum;                   /* section number */
	unsigned short n_type;           /* type and derived type */
	char n_sclass;                   /* storage class */
	char n_numaux;                   /* number of aux. entries */
};

struct x_file {
	union {
		char _x_fname[EFILNMLEN]; /* file name */
		struct {
			long x_zeroes; /* if _x_fname[0-3] == 0 */
			long x_offset; /* offset into string table */
		} _x_n;
	} _x_file;
};

struct x_tag {
	char zeroes0[6];
	unsigned short x_size;  /* size of struct, union or enum */
	char zeroes1[4];
	long x_endndx;          /* index of next entry for the tag */
};

struct x_etag {
	long xtagndx;           /* tag index */
	char zeroes[2];
	unsigned short x_size;  /* size of struct, union or enum */
};

struct x_scn {
	long x_scnlen;              /* section length */
	unsigned short x_nreloc;    /* num reloc entries */
	unsigned short x_nlinno;    /* num line numbers */
	unsigned long x_checksum;   /* section COMDAT checksum for PE */
	unsigned short x_associated; /* COMDAT associated section index for PE */
	unsigned char x_comdat;      /* COMDAT selection number for PE */
};

struct x_fun {
	long x_tagndx;         /* tag index */
	long x_fsize;          /* size in bytes */
	long x_lnnoptr;        /* file pointer to line info */
	long x_endndx;         /* index to next entry */
	unsigned short x_tvndx; /* index of function in vector table */
};

struct x_ary {
	long x_tagndx;               /* tag index */
	unsigned short x_lnno;       /* line number of declaration */
	unsigned short x_size;       /* size of array */
	unsigned short x_dimen[4];   /* until 4 dimensions */
};

union auxent {
	struct x_fun x_fun;
	struct x_ary x_ary;
	struct x_scn x_scn;
	struct x_tag x_tag;
	struct x_etag x_etag;
	struct x_file x_file;
};

/* Special n_scnum values */
#define N_DEBUG      -2
#define N_ABS        -1
#define N_UNDEF       0
#define N_SCNUM(x)   ((x) > 0)

/* basic types */
#define T_NULL        0
#define T_VOID        1
#define T_CHAR        2
#define T_SHORT       3
#define T_INT         4
#define T_LONG        5
#define T_FLOAT       6
#define T_DOUBLE      7
#define T_STRUCT      8
#define T_UNION       9
#define T_ENUM       10
#define T_MOE        11
#define T_UCHAR      12
#define T_USHORT     13
#define T_UINT       14
#define T_ULONG      15
#define T_LNGDBL     16

/* derivated types */
#define DT_NON       0
#define DT_PTR       1
#define DT_FCN       2
#define DT_ARY       3

/* storage class */
#define C_NULL       0
#define C_AUTO       1
#define C_EXT        2
#define C_STAT       3
#define C_REG        4
#define C_EXTDEF     5
#define C_LABEL      6
#define C_ULABEL     7
#define C_MOS        8
#define C_ARG        9
#define C_STRTAG     10
#define C_MOU        11
#define C_UNTAG      12
#define C_TPDEF      13
#define C_USTATIC    14
#define C_ENTAG      15
#define C_MOE        16
#define C_REGPARM    17
#define C_FIELD      18
#define C_AUTOARG    19
#define C_LASTENT    20
#define C_BLOCK      100
#define C_FCN        101
#define C_EOS        102
#define C_FILE       103
#define C_LINE       104
#define C_ALIAS      105
#define C_HIDDEN     106
#define C_WEAKEXT    127
#define C_EFCN       255
