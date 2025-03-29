/*
 * This file is inspired in:
 *	- the book "Understanding and using COFF"
 *	- the book "UNIX System V 386 R3.2 Programmers guide, Vol 2"
 */

/* Special n_scnum values */
#define N_TV         -3
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

#define SYMNMLEN   8
#define FILNMLEN  14

#define SYMENT  struct syment
#define SYMESZ  18

#define AUXENT  union auxent
#define AUXESZ  18

#define n_name       _n._n_name
#define n_zeroes     _n._n_n._n_zeroes
#define n_offset     _n._n_n._n_offset
#define n_nptr       _n._n_nptr[1]

#define E_FILNMLEN 18
#define DIMNUM     4

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

#define x_fname    _x_file._x_fname
#define x_zeroes   _x_file._x_n._x_zeroes
#define x_offset   _x_file._x_n._x_offset
#define x_tagndx   _x_sym._x_tagndx
#define x_lnno     _x_sym._x_misc._x_lnsz._x_lnno
#define x_size     _x_sym._x_misc._x_lnsz._x_size
#define x_fsize    _x_sym._x_misc._x_fsize
#define x_lnnoptr  _x_sym._x_fcnary._x_fcn._x_lnnoptr
#define x_endndx   _x_sym._x_fcnary._x_fcn._x_endndx
#define x_dimen    _x_sym._x_fcnary._x_ary._x_dimen
#define x_tvndx    _x_sym._x_tvndx
#define x_scnlen   _x_scn._x_scnlen
#define x_nreloc   _x_scn._x_nreloc
#define x_nlinno   _x_scn._x_nlinno
#define x_checksum _x_scn._x_checksum
#define x_associated _x_scn._x_associated
#define x_comdat   _x_scn._x_comdat
#define x_tvfill   _x_tv._x_tvfill
#define x_tvlen    _x_tv._x_tvlen
#define x_tvran    _x_tv._x_tvran

union auxent {
	union {
		char _x_fname[E_FILNMLEN]; /* file name */
		struct {
			long _x_zeroes; /* if _x_fname[0-3] == 0 */
			long _x_offset; /* offset into string table */
		} _x_n;
	} _x_file;

	struct {
		long _x_tagndx;	/* str, un, or enum tag indx */
		union {
			struct {
				unsigned short _x_lnno;	/* declaration line number */
				unsigned short _x_size;	/* str, union, array size */
			} _x_lnsz;
			long _x_fsize; /* size of function */
		} _x_misc;

		union {
			struct {             /* if ISFCN, tag, or .bb */
				long _x_lnnoptr;  /* ptr to fcn line # */
				long _x_endndx;   /* entry ndx past block end */
			} _x_fcn;
			struct {             /* if ISARY, up to 4 dimen. */
				unsigned short _x_dimen[DIMNUM];
			} _x_ary;
		} _x_fcnary;

		unsigned short _x_tvndx;		/* tv index */
	} _x_sym;

	struct {
		long _x_scnlen;                /* section length */
		unsigned short _x_nreloc;      /* number of relocation entries */
		unsigned short _x_nlinno;      /* number of line numbers */
		unsigned long _x_checksum;     /* section COMDAT checksum         */
		unsigned short _x_associated;  /* COMDAT associated section index */
		unsigned char _x_comdat;       /* COMDAT selection number         */
	} _x_scn;

	struct {	   /* info about .tv section (in auxent of symbol .tv)) */
		long _x_tvfill;	             /* tv fill value */
		unsigned short _x_tvlen;     /* length of .tv */
		unsigned short _x_tvran[2];  /* tv range */
	} _x_tv;
};
