#include <stdio.h>

#include <scc/mach.h>
#include <scc/coff32.h>

#include "../libmach.h"

struct arch coff32archs[] = {
	{"coff-i386", "\x4c\x01", OBJ(COFF32, ARCH386, LITTLE_ENDIAN), 0x0105},
	{"coff-z80", "\x5a\x80", OBJ(COFF32, ARCHZ80, LITTLE_ENDIAN), 0x3105},
	NULL,
};
