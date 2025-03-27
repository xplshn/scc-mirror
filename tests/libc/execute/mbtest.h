static wchar_t wc;
static struct mbtest {
	char *s;
	int l;
	int rmbr;
	int rmb;
	int mbstate;
	int syserr;
	wchar_t *pwc;
	wchar_t wc;
} tests[] = {
	{"\0",                             2,  0,  0, 1, 0,      &wc,  0},
	{"\x21",                           2,  1,  1, 1, 0,      &wc,  0x21},
	{"\xc2\xa1",                       3,  2,  2, 1, 0,      &wc,  0x00A1},
	{"\xc2\xa1",                       2,  2,  2, 1, 0,      &wc,  0x00A1},
	{"\xe2\x80\x94",                   4,  3,  3, 1, 0,      &wc,  0x2014},
	{"\xf0\x9f\x92\xa9",               5,  4,  4, 1, 0,      &wc,  0x01F4A9},
	{"\xf0\x9f\x92\xa9",               5,  4,  4, 1, 0,      NULL, -1},
	{"\xf0\x9f\x92\xa9",              -1,  4,  4, 1, 0,      &wc,  0x01F4A9},

	{NULL,                             4,  0,  0, 1, 0,      NULL, -1},
	{"\xed\x9f\xbf",                   4,  3,  3, 1, 0,      &wc,  0xd7ff},
	{"\xed\xa0\x80",                   4, -1, -1, 1, EILSEQ, &wc,  -1},
	{"\xed\xb3\xbf",                   4, -1, -1, 1, EILSEQ, &wc,  -1},
	{"\xed\xb4\x80",                   4,  3,  3, 1, 0,      &wc,  0xdd00},

	{"\xf0\x9f\x92\xa9",               3, -2, -1, 0, 0,      &wc, -1},
	{"\xa9",                           2,  1, -1, 1, 0,      &wc, 0x01F4A9},
	{"\xf0\x9f\x92\xa9",               3, -2, -1, 0, 0,      &wc, -1},
	{NULL,                             4, -1,  0, 1, EILSEQ, &wc, -1},
	{"\xa9",                           2, -1, -1, 1, EILSEQ, &wc, -1},
	{"\xf0\x9f\x92\xa9",               3, -2, -1, 0, 0,      &wc, -1},
	{NULL,                             4, -1,  0, 1, EILSEQ, &wc, -1},
	{"\x21",                           2,  1,  1, 1, 0,      &wc,  0x21},
	{"\xf0\x9f\x92\xa9",               2, -2, -1, 0, 0,      &wc, -1},
	{"\xf0\x9f\x92\xa9",               0, -2, -1, 0, 0,      &wc, -1},
	{"\x92\xa9",                       2,  2, -1, 1, 0,      &wc, 0x01F4A9},

	{"\x80",                           2, -1, -1, 1, EILSEQ, &wc, -1},
	{"\xc0\x80",                       2, -1, -1, 1, EILSEQ, &wc, -1},
	{"\xc0\x00",                       2, -1, -1, 1, EILSEQ, &wc, -1},
	{"\xc1\x81",                       2, -1, -1, 1, EILSEQ, &wc, -1},
	{"\xf8\x81\x82\x83\x84\x85",      -1, -1, -1, 1, EILSEQ, &wc, -1},
	{"\xfe\x81\x82\x83\x84\x85\x86",   8, -1, -1, 1, EILSEQ, &wc, -1},
};
