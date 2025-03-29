#include <stdio.h>

#include <scc/mach.h>
#include <scc/elf/elftypes.h>
#include <scc/elf/elfhdr.h>
#include <scc/elf.h>

#include "../libmach.h"
#include "fun.h"

int
elfprobe(unsigned char *buf, char **name)
{
	int arch, data, endian;
	struct elfhdr32 hdr;
	struct arch *ap;

	data = buf[EI_DATA];
	endian = (data == ELFDATA2LSB) ? LITTLE_ENDIAN : BIG_ENDIAN;
	arch = (buf[EI_CLASS] == ELFCLASS32) ? ARCHUNK32 : ARCHUNK64;

	unpack(endian,
	       buf,
	       "'16sss",
	       hdr.e_ident,
	       &hdr.e_type,
	       &hdr.e_machine,
	       &hdr.e_version);

	if (!IS_ELF(hdr)
	||  buf[EI_VERSION] != EV_CURRENT
	||  hdr.e_version != EV_CURRENT
	||  buf[EI_CLASS] != ELFCLASS32 && buf[EI_CLASS] != ELFCLASS64
	||  (data != ELFDATA2LSB && data != ELFDATA2MSB)) {
		return -1;
	}

	for (ap = elfarchs; ap->name; ap++) {
		if (ap->mach == hdr.e_machine &&  ap->endian == data) {
			if (name)
				*name = ap->name;
			return ap->type;
		}
	}

	if  (name)
		*name = "elf-unknown";

	return OBJ(ELF, arch, endian);
}
