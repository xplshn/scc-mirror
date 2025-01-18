#include <stdio.h>

#include <scc/mach.h>
#include <scc/elf64.h>

#include "../libmach.h"

struct arch elf64archs[] = {
	{
		.name = "elf-amd64",
		.mach = EM_X86_64,
		.endian = ELFDATA2LSB,
		.type = OBJ(ELF64, ARCHAMD64, LITTLE_ENDIAN),
	},
	NULL,
};
