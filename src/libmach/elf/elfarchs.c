#include <stdio.h>


#include <scc/mach.h>
#include <scc/elf/elftypes.h>
#include <scc/elf/elfhdr.h>
#include <scc/elf.h>

#include "../libmach.h"

struct arch elfarchs[] = {
	{
		.name = "elf-amd64",
		.mach = EM_X86_64,
		.endian = ELFDATA2LSB,
		.type = OBJ(ELF, ARCHAMD64, LITTLE_ENDIAN),
		.is32 = 0,
	},
	NULL,
};
