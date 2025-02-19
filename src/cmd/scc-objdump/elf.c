#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/elf/elftypes.h>
#include <scc/elf/elfhdr.h>
#include <scc/elf/elfphdr.h>
#include <scc/elf/elfshdr.h>
#include <scc/elf/elfent.h>
#include <scc/elf.h>

#include "objdump.h"

enum elfsecflags {
	ELF_WRITE = 0,
	ELF_ALLOC = 1,
	ELF_EXEC = 2,
	ELF_MERGE = 3,
	ELF_STRINGS = 4,
	ELF_INFO_LINK = 5,
	ELF_LINK_ORDER = 6,
	ELF_OS_NONCONFOR = 7,
	ELF_GROUP = 8,
	ELF_TLS = 9,
	ELF_COMPRESSED = 10,
	ELF_NR_FLAGS = 11,
};

enum phdrflags {
	FLAG_X = 0,
	FLAG_W = 1,
	FLAG_R = 2,
	NR_RIGHTS = 3,
};

int
elfhasrelloc(Obj *obj, Section *sec)
{
	size_t i;
	Elf *elf = obj->data;
	Elfsec *shdr;

	for (i = 0; i < elf->nsec; i++) {
		shdr = &elf->secs[i];
		if (shdr->type != SHT_RELA && shdr->type != SHT_REL)
			continue;
		if (shdr->info == sec->index)
			return 1;
	}

	return 0;
}

static void
printents(Obj *obj)
{
	int n;
	size_t i;
	Section sec;
	Elfsym *ent;
	Elf *elf = obj->data;
	char *sbind, *stype, *svis, *ssec;
	unsigned info, bind, type, vis, nsec;

	static char *binds[] = {
		[STB_LOCAL] = "Local symbol",
		[STB_GLOBAL] = "Global symbol",
		[STB_WEAK] = "like global - lower precedence",
		[STB_NUM] = "number of symbol bindings",
		[STB_LOPROC] = "reserved range for processor",
		[STB_HIPROC] = " specific symbol bindings",
	};
	static char *types[] = {
		[STT_NOTYPE] = "not specified",
		[STT_OBJECT] = "data object",
		[STT_FUNC] = "function",
		[STT_SECTION] = "section",
		[STT_FILE] = "file",
		[STT_COMMON] = "common symbol",
		[STT_TLS] = "thread local storage",
		[STT_LOPROC] = "reserved range for processor",
		[STT_HIPROC] = " specific symbol types",
	};
	static char *visibilities[] = {
		[STV_DEFAULT] = "Visibility set by binding type",
		[STV_INTERNAL] = "OS specific version of STV_HIDDEN",
		[STV_HIDDEN] = "can only be seen inside own .so",
		[STV_PROTECTED] = "HIDDEN inside, DEFAULT outside",
	};

	for (i = 0; i < elf->nsym; i++) {
		ent = &elf->syms[i];

		info = ent->info;
		bind = ELF_ST_BIND(info);
		type = ELF_ST_TYPE(info);
		vis = ELF_ST_VISIBILITY(ent->other);
		nsec = ent->shndx;

		sbind = (bind <= STB_HIPROC) ? binds[bind] : "Unknown";
		stype = (type <= STT_HIPROC) ? types[type] : "Unknown";
		svis = (vis <= STV_PROTECTED) ? visibilities[vis] : "Unknown";
		if (!sbind)
			sbind = "Unknown";
		if (!stype)
			stype = "Unknown";
		if (!svis)
			svis = "Unknown";

		switch (nsec) {
		case SHN_ABS:
			ssec = "*ABS*";
			break;
		case SHN_COMMON:
			ssec = "*COM*";
			break;
		default:
			n = nsec;
			ssec = "*UNK*";
			if (getsec(obj, &n, &sec))
				ssec = sec.name;
		}

		printf("Symbol %zu:\n"
		        "\tst_name: %lu '%s'\n"
		        "\tst_info: %u\n"
		        "\t\tst_bind: %u %s\n"
		        "\t\tst_type: %u %s\n"
		        "\tst_other: %u %s\n"
		        "\tst_shndx: %u %s\n"
		        "\tst_value: %#llx\n"
		        "\tst_size: %llu\n"
		        "\n",
		        i,
		        (long) ent->st_name, ent->name,
		        info,
		        bind, sbind,
		        type, stype,
		        vis, svis,
		        nsec, ssec,
		        (unsigned long long) ent->value,
		        (unsigned long long) ent->size);
	}
}

static void
printstbl(Obj *obj)
{
	int n;
	size_t i;
	Symbol sym;
	Section sec;
	Elfsym *ent;
	Elfsec *shdr;
	Elf *elf = obj->data;
	unsigned info, bind, type;
	char cbind, cweak, cctor, cwarn, cindir, cdebug, ctype;

	if (elf->nsym == 0) {
		puts("no symbols");
		return;
	}

	for (i = 1; i < elf->nsym; i++) {
		ent = &elf->syms[i];
		shdr =&elf->secs[ent->shndx];
		n = i;
		getsym(obj, &n, &sym);
		n = ent->shndx;
		getsec(obj, &n, &sec);

		info = ent->info;
		bind = ELF64_ST_BIND(info);
		type = ELF64_ST_TYPE(info);

		cbind = (bind == STB_LOCAL) ? 'l' : 'g';
		cweak = (bind == STB_WEAK) ? 'w' : ' ';
		cctor = ' ';
		cwarn = ' ';
		cindir = ' ';

		switch (sym.type) {
		case 'N':
		case 'n':
			cdebug = 'd';
			break;
		case 'U':
			cdebug = ' ';
			cbind = ' ';
			break;
		default:
			cdebug = (ent->symsec->type  == SHT_DYNAMIC) ? 'D' : ' ';
		}

		switch (type) {
		case STT_OBJECT:
			ctype = 'O';
			break;
		case STT_FUNC:
			ctype = 'F';
			break;
		case STT_FILE:
			ctype = 'f';
			cdebug = 'd';
			break;
		default:
			ctype = ' ';
			break;
		}

		printf("%016llx %c%c%c%c%c%c%c %-15s %08llu %-20s [%4zu]\n",
		       (long long) ent->value,
		       cbind,
		       cweak,
		       cctor,
		       cwarn,
		       cindir,
		       cdebug,
		       ctype,
		       sec.name,
		       (long long) ent->size,
		       sym.name,
		       i);
	}
}

void
elfsyms(Obj *obj)
{
	printstbl(obj);

	if (pflag)
		printents(obj);
}

void
elfscns(Obj *obj)
{
	size_t i;
	Elf *elf = obj->data;
	Elfsec *shdr;
	static char *types[] = {
		[SHT_NULL] = "inactive",
		[SHT_PROGBITS] = "program defined information",
		[SHT_SYMTAB] = "symbol table section",
		[SHT_STRTAB] = "string table section",
		[SHT_RELA] = "relocation section with addends",
		[SHT_HASH] = "symbol hash table section",
		[SHT_DYNAMIC] = "dynamic section",
		[SHT_NOTE] = "note section",
		[SHT_NOBITS] = "no space section",
		[SHT_REL] = "relation section without addends",
		[SHT_SHLIB] = "reserved - purpose unknown",
		[SHT_DYNSYM] = "dynamic symbol table section",
		[SHT_NUM] = "number of section types",
		[SHT_INIT_ARRAY] = "pointers to init functions",
		[SHT_FINI_ARRAY] = "pointers to termination functions",
		[SHT_PREINIT_ARRAY] = "ptrs to funcs called before init",
		[SHT_GROUP] = "defines a section group",
		[SHT_SYMTAB_SHNDX] = "Section indexes (see SHN_XINDEX).",
	};
	static Flags f = {
		.nr = ELF_NR_FLAGS,
		.text = {
			[ELF_WRITE] = "WRITE",
			[ELF_ALLOC] = "ALLOC",
			[ELF_EXEC] = "EXEC",
			[ELF_MERGE] = "MERGE",
			[ELF_STRINGS] = "STRINGS",
			[ELF_INFO_LINK] = "INFO_LINK",
			[ELF_LINK_ORDER] = "LINK_ORDER",
			[ELF_OS_NONCONFOR] = "OS_NONCONFORMING",
			[ELF_GROUP] = "GROUP",
			[ELF_TLS] = "TLS",
			[ELF_COMPRESSED] = "COMPRESSED",
		}
	};

	for (i = 0; i < elf->nsec; i++) {
		long type;
		char *stype;
		shdr = &elf->secs[i];

		type = shdr->type;
		if (type <= SHT_SYMTAB_SHNDX) {
			stype = types[type];
		} else {
			switch (type) {
			case SHT_SUNW_dof:
				stype = "SHT_SUNW_dof";
				break;
			case SHT_GNU_LIBLIST:
				stype = "SHT_GNU_LIBLIST";
				break;
			case SHT_SUNW_move:
				stype = "SHT_SUNW_move";
				break;
			case SHT_SUNW_syminfo:
				stype = "SHT_SUNW_syminfo";
				break;
			case SHT_GNU_VERDEF:
				stype = "SHT_GNU_VERDEF";
				break;
			case SHT_GNU_VERNEED:
				stype = "SHT_GNU_VERNEED";
				break;
			case SHT_GNU_VERSYM:
				stype = "SHT_GNU_VERSYM";
				break;
			default:
				stype = NULL;
			}
		}

		if (!stype)
			stype = "Unknown";

		f.flags = shdr->flags;

		printf("Section %zu:\n"
		       "\tsh_name: %lu %s\n"
		       "\tsh_type: %lu %s\n"
		       "\tsh_flags: %#llx\n"
		       "\tsh_addr: %#llx\n"
		       "\tsh_offset: %#llx\n"
		       "\tsh_size: %#llx\n"
		       "\tsh_link: %lu\n"
		       "\tsh_info: %lu\n"
		       "\tsh_addralign: %llu\n"
		       "\tsh_entsize: %llu\n",
		       i,
		       (long) shdr->sh_name, shdr->name,
		       type, stype,
		       (long long) shdr->flags,
		       (long long) shdr->addr,
		       (long long) shdr->offset,
		       (long long) shdr->size,
		       (long) shdr->link,
		       (long) shdr->info,
		       (long long) shdr->addralign,
		       (long long) shdr->entsize);

		putchar('\t');
		printflags(&f);
		putchar('\n');
	}
}

static void
printfhdr(Elfhdr *hdr)
{
	unsigned long version;
	unsigned class, data, abi, type, mach;
	char *sclass, *sdata, *sabi, *stype, *smach, *sversion;

	static char *abis[] = {
		[ELFOSABI_SYSV] = "UNIX System V ABI",
		[ELFOSABI_HPUX] = "HP-UX operating system",
		[ELFOSABI_NETBSD] = "NetBSD",
		[ELFOSABI_LINUX] = "GNU/Linux",
		[ELFOSABI_HURD] = "GNU/Hurd",
		[ELFOSABI_86OPEN] = "86Open common IA32 ABI",
		[ELFOSABI_SOLARIS] = "Solaris",
		[ELFOSABI_MONTEREY] = "Monterey",
		[ELFOSABI_IRIX] = "IRIX",
		[ELFOSABI_FREEBSD] = "FreeBSD",
		[ELFOSABI_TRU64] = "TRU64 UNIX",
		[ELFOSABI_MODESTO] = "Novell Modesto",
		[ELFOSABI_OPENBSD] = "OpenBSD",
		[ELFOSABI_OPENVMS] = "Open VMS",
		[ELFOSABI_NSK] = "Hewlett-Packard Non-Stop Kernel",
		[ELFOSABI_AROS] = "Amiga Research OS",
		[ELFOSABI_FENIXOS] = "The FenixOS multi-core OS",
		[ELFOSABI_CLOUDABI] = "Nuxi CloudABI",
		[ELFOSABI_OPENVOS] = "Stratus Technologies OpenVOS",
		[ELFOSABI_ARM] = "ARM",
		[ELFOSABI_STANDALONE] = "Standalone (embedded) application",
	};
	static char *classes[] = {
		[ELFCLASSNONE] = "invalid",
		[ELFCLASS32] = "32-bit objs",
		[ELFCLASS64] = "64-bit objs",
	};
	static char *datas[] = {
		[ELFDATANONE] = "invalid",
		[ELFDATA2LSB] = "Little-Endian",
		[ELFDATA2MSB] = "Big-Endian",
	};
	static char *types[] = {
		[ET_NONE] = "No file type",
		[ET_REL] = "Relocatable file",
		[ET_EXEC] = "Executable file",
		[ET_DYN] = "Shared object file",
		[ET_CORE] = "Core file",
	};
	static char *machs[] = {
		[EM_NONE] = "No machine",
		[EM_M32] = "AT&T WE 32100",
		[EM_SPARC] = "SPARC",
		[EM_386] = "Intel 80386",
		[EM_68K] = "Motorola 68000",
		[EM_88K] = "Motorola 88000",
		[EM_IAMCU] = "Intel MCU",
		[EM_860] = "Intel 80860",
		[EM_MIPS] = "MIPS I Architecture",
		[EM_S370] = "IBM System/370 Processor",
		[EM_MIPS_RS3_LE] = "MIPS RS3000 Little-endian",
		[EM_PARISC] = "Hewlett-Packard PA-RISC",
		[EM_VPP500] = "Fujitsu VPP500",
		[EM_SPARC32PLUS] = "Enhanced instruction set SPARC",
		[EM_960] = "Intel 80960",
		[EM_PPC] = "PowerPC",
		[EM_PPC64] = "64-bit PowerPC",
		[EM_S390] = "IBM System/390",
		[EM_SPU] = "IBM SPU/SPC",
		[EM_V800] = "NEC V800",
		[EM_FR20] = "Fujitsu FR20",
		[EM_RH32] = "TRW RH-32",
		[EM_RCE] = "Motorola RCE",
		[EM_ARM] = "ARM AARCH32",
		[EM_ALPHA] = "Digital Alpha",
		[EM_SH] = "Hitachi SH",
		[EM_SPARCV9] = "SPARC Version 9",
		[EM_TRICORE] = "Siemens TriCore",
		[EM_ARC] = "Argonaut RISC Core",
		[EM_H8_300] = "Hitachi H8/300",
		[EM_H8_300H] = "Hitachi H8/300H",
		[EM_H8S] = "Hitachi H8S",
		[EM_H8_500] = "Hitachi H8/500",
		[EM_IA_64] = "Intel IA-64",
		[EM_MIPS_X] = "Stanford MIPS-X",
		[EM_COLDFIRE] = "Motorola ColdFire",
		[EM_68HC12] = "Motorola M68HC12",
		[EM_MMA] = "Fujitsu MMA",
		[EM_PCP] = "Siemens PCP",
		[EM_NCPU] = "Sony nCPU",
		[EM_NDR1] = "Denso NDR1",
		[EM_STARCORE] = "Motorola Star*Core",
		[EM_ME16] = "Toyota ME16",
		[EM_ST100] = "STMicroelectronics ST100",
		[EM_TINYJ] = "Advanced Logic Corp. TinyJ",
		[EM_X86_64] = "AMD x86-64",
		[EM_PDSP] = "Sony DSP Processor",
		[EM_PDP10] = "DEC PDP-10",
		[EM_PDP11] = "DEC PDP-11",
		[EM_FX66] = "Siemens FX66",
		[EM_ST9PLUS] = "STMicroelectronics ST9+",
		[EM_ST7] = "STMicroelectronics ST7",
		[EM_68HC16] = "Motorola MC68HC16",
		[EM_68HC11] = "Motorola MC68HC11",
		[EM_68HC08] = "Motorola MC68HC08",
		[EM_68HC05] = "Motorola MC68HC05",
		[EM_SVX] = "Silicon Graphics SVx",
		[EM_ST19] = "STMicroelectronics ST19",
		[EM_VAX] = "Digital VAX",
		[EM_CRIS] = "Axis Communications 32-bit",
		[EM_JAVELIN] = "Infineon Technologies 32-bit",
		[EM_FIREPATH] = "Element 14 64-bit DSP Processor",
		[EM_ZSP] = "LSI Logic 16-bit DSP Processor",
		[EM_MMIX] = "Donald Knuth's educational 64-bit",
		[EM_HUANY] = "Harvard machine-independent",
		[EM_PRISM] = "SiTera Prism",
		[EM_AVR] = "Atmel AVR 8-bit",
		[EM_FR30] = "Fujitsu FR30",
		[EM_D10V] = "Mitsubishi D10V",
		[EM_D30V] = "Mitsubishi D30V",
		[EM_V850] = "NEC v850",
		[EM_M32R] = "Mitsubishi M32R",
		[EM_MN10300] = "Matsushita MN10300",
		[EM_MN10200] = "Matsushita MN10200",
		[EM_PJ] = "picoJava",
		[EM_OPENRISC] = "OpenRISC 32-bit",
		[EM_ARC_A5] = "ARC ARCompact",
		[EM_ARC_COMPACT] = "ARC ARCompact",
		[EM_XTENSA] = "Tensilica Xtensa",
		[EM_VIDEOCORE] = "Alphamosaic VideoCore",
		[EM_TMM_GPP] = "Thompson Multimedia GPP",
		[EM_NS32K] = "National 32000 series",
		[EM_TPC] = "Tenor Network TPC",
		[EM_SNP1K] = "Trebia SNP 1000",
		[EM_ST200] = "STMicroelectronics ST200",
		[EM_IP2K] = "Ubicom IP2xxx",
		[EM_MAX] = "MAX Processor",
		[EM_CR] = "National CompactRISC",
		[EM_F2MC16] = "Fujitsu F2MC16",
		[EM_MSP430] = "Texas msp430",
		[EM_BLACKFIN] = "Analog Devices Blackfin",
		[EM_SE_C33] = "S1C33 of Seiko Epson",
		[EM_SEP] = "Sharp embedded",
		[EM_ARCA] = "Arca RISC",
		[EM_UNICORE] = "PKU-Unity Ltd. and MPRC",
		[EM_EXCESS] = "eXcess CPU",
		[EM_DXP] = "Deep Execution Processor",
		[EM_ALTERA_NIOS2] = "Altera Nios II",
		[EM_CRX] = "National CompactRISC CRX",
		[EM_XGATE] = "Motorola XGATE",
		[EM_C166] = "Infineon C16x/XC16x",
		[EM_M16C] = "Renesas M16C",
		[EM_DSPIC30F] = "Microchip dsPIC30F",
		[EM_CE] = "Freescale Communication Engine",
		[EM_M32C] = "Renesas M32C",
		[EM_TSK3000] = "Altium TSK3000 core",
		[EM_RS08] = "Freescale RS08",
		[EM_SHARC] = "Analog Devices SHARC",
		[EM_ECOG2] = "Cyan Technology eCOG2",
		[EM_SCORE7] = "Sunplus S+core7",
		[EM_DSP24] = "NJR 24-bit DSP",
		[EM_VIDEOCORE3] = "Broadcom VideoCore III",
		[EM_LATTICEMICO3] = "RISC processor for Lattice FPGA",
		[EM_SE_C17] = "Seiko Epson C17",
		[EM_TI_C6000] = "TMS320C6000 DSP family",
		[EM_TI_C2000] = "TMS320C2000 DSP family",
		[EM_TI_C5500] = "TMS320C55x DSP family",
		[EM_TI_ARP32] = "Texas Application Specific RISC",
		[EM_TI_PRU] = "Texas Programmable Realtime Unit",
		[EM_MMDSP_PLUS] = "STMicroelectronics 64bit VLIW",
		[EM_CYPRESS_M8C] = "Cypress M8C microprocessor",
		[EM_R32C] = "Renesas R32C series",
		[EM_TRIMEDIA] = "NXP Semiconductors TriMedia",
		[EM_QDSP6] = "QUALCOMM DSP6 Processor",
		[EM_8051] = "Intel 8051 and variants",
		[EM_STXP7X] = "STMicroelectronics STxP7x",
		[EM_NDS32] = "Andes Technology embedded RISC",
		[EM_ECOG1] = "Cyan Technology eCOG1X family",
		[EM_ECOG1X] = "Cyan Technology eCOG1X family",
		[EM_MAXQ30] = "MAXQ30 Core Micro-controllers",
		[EM_XIMO16] = "NJR 16-bit DSP Processor",
		[EM_MANIK] = "M2000 Reconfigurable RISC",
		[EM_CRAYNV2] = "Cray Inc. NV2 vector architecture",
		[EM_RX] = "Renesas RX family",
		[EM_METAG] = "Imagination Technologies META",
		[EM_MCST_ELBRUS] = "MCST Elbrus",
		[EM_ECOG16] = "Cyan Technology eCOG16 family",
		[EM_CR16] = "National CompactRISC CR16",
		[EM_ETPU] = "Freescale Extended Time Unit",
		[EM_SLE9X] = "Infineon Technologies SLE9X core",
		[EM_L10M] = "Intel L10M",
		[EM_K10M] = "Intel K10M",
		[EM_AARCH64] = "ARM AARCH64",
		[EM_AVR32] = "Atmel 32-bit",
		[EM_STM8] = "STMicroeletronics STM8 ",
		[EM_TILE64] = "Tilera TILE64",
		[EM_TILEPRO] = "Tilera TILEPro",
		[EM_MICROBLAZE] = "Xilinx MicroBlaze 32-bit",
		[EM_CUDA] = "NVIDIA CUDA architecture",
		[EM_TILEGX] = "Tilera TILE-Gx family",
		[EM_CLOUDSHIELD] = "CloudShield architecture family",
		[EM_COREA_1ST] = "KIPO-KAIST Core-A 1st gen family",
		[EM_COREA_2ND] = "KIPO-KAIST Core-A 2nd gen family",
		[EM_ARC_COMPACT2] = "Synopsys ARCompact V2",
		[EM_OPEN8] = "Open8 8-bit RISC soft processor core",
		[EM_RL78] = "Renesas RL78 family",
		[EM_VIDEOCORE5] = "Broadcom VideoCore V processor",
		[EM_78KOR] = "Renesas 78KOR family",
		[EM_56800EX] = "Freescale 56800EX (DSC)",
		[EM_BA1] = "Beyond BA1 CPU architecture",
		[EM_BA2] = "Beyond BA2 CPU architecture",
		[EM_XCORE] = "XMOS xCORE processor family",
		[EM_MCHP_PIC] = "Microchip 8-bit PIC(r) family",
		[EM_KM32] = "KM211 KM32 32-bit processor",
		[EM_KMX32] = "KM211 KMX32 32-bit processor",
		[EM_KMX16] = "KM211 KMX16 16-bit processor",
		[EM_KMX8] = "KM211 KMX8 8-bit processor",
		[EM_KVARC] = "KM211 KVARC processor",
		[EM_CDP] = "Paneve CDP architecture family",
		[EM_COGE] = "Cognitive Smart Memory Processor",
		[EM_COOL] = "Bluechip Systems CoolEngine",
		[EM_NORC] = "Nanoradio Optimized RISC",
		[EM_CSR_KALIMBA] = "CSR Kalimba architecture family",
		[EM_Z80] = "Zilog Z80",
		[EM_VISIUM] = "VISIUMcore processor",
		[EM_FT32] = "FTDI Chip FT32",
		[EM_MOXIE] = "Moxie processor family",
		[EM_AMDGPU] = "AMD GPU architecture",
		[EM_RISCV] = "RISC-V",
		[EM_BPF] = "Linux BPF",
		[EM_CSKY] = "C-SKY",
	};
	static char *versions[] = {
		[EV_NONE] = "Invalid",
		[EV_CURRENT] = "Current",
	};

	class = hdr->ident[EI_CLASS];
	data = hdr->ident[EI_DATA];
	abi = hdr->ident[EI_OSABI];
	type = hdr->type;
	mach = hdr->machine;
	version = hdr->version;

	sclass = (class <= ELFCLASS64) ? classes[class] : "Unknown";
	sdata = (data <= ELFDATA2MSB) ? datas[data] : "Unknown";
	stype = (type <= ET_CORE) ? types[type] : "Unknown";
	smach = (mach <= EM_CSKY) ? machs[mach] : "Unknown";
	if (!smach)
		smach = "Unknown";
	sversion = (version <= EV_CURRENT) ? versions[version] : "Unknown";

	switch (abi) {
	case ELFOSABI_ARM:
		sabi = "ARM";
		break;
	case ELFOSABI_STANDALONE:
		sabi = "Standalone (embedded) application";
		break;
	default:
		sabi = (abi <= ELFOSABI_OPENVOS) ? abis[abi] : "Unknown";
	}

	printf("elfhdr:\n"
	       "\tei_class: %u, %s\n"
	       "\tei_data: %u, %s\n"
	       "\tei_version: %u\n"
	       "\tei_osabi: %u, %s\n"
	       "\tei_abiversion: %u\n"
	       "\te_type: %u, %s\n"
	       "\te_machine: %u, %s\n"
	       "\te_version: %lu, %s\n"
	       "\te_entry: 0x%08llx\n"
	       "\te_phoff: %llu\n"
	       "\te_shoff: %llu\n"
	       "\te_flags: %#lx\n"
	       "\te_ehsize: %lu\n"
	       "\te_phentsize: %lu\n"
	       "\te_phnum: %lu\n"
	       "\te_shentsize: %lu\n"
	       "\te_shnum: %lu\n"
	       "\te_shstrndx: %lu\n"
	       "\n",
	       class, sclass,
	       data, sdata,
	       hdr->ident[EI_VERSION],
	       abi, sabi,
	       hdr->ident[EI_ABIVERSION],
	       type, stype,
	       mach, smach,
	       version, sversion,
	       (long long) hdr->entry,
	       (long long) hdr->phoff,
	       (long long) hdr->shoff,
	       (long) hdr->flags,
	       (long) hdr->ehsize,
	       (long) hdr->phentsize,
	       (long) hdr->phnum,
	       (long) hdr->shentsize,
	       (long) hdr->shnum,
	       (long) hdr->shstrndx);
}

static void
printphdr(Obj *obj, Elfphdr *phdr, unsigned long n)
{
	int j;
	Map *map;
	Mapsec *seg;
	unsigned long i;
	static char *types[] = {
		[PT_NULL] = "unused",
		[PT_LOAD] = "loadable segment",
		[PT_DYNAMIC] = "dynamic linking section",
		[PT_INTERP] = "the RTLD",
		[PT_NOTE] = "note information",
		[PT_SHLIB] = "reserved - purpose undefined",
		[PT_PHDR] = "program header",
		[PT_TLS] = "thread local storage",
	};
	static Flags f ={
		.nr = NR_RIGHTS,
		.text = {
			[FLAG_X] = "Executable",
			[FLAG_W] = "Writable",
			[FLAG_R] = "Readable",
		}
	};

	if ((map = loadmap(obj, NULL)) == NULL) {
		error("getting the object memory map");
		return;
	}

	for (i = 0; i < n; i++) {
		char *stype;
		unsigned long type;

		f.flags = phdr->flags;
		type = phdr->type;
		if (type <= PT_TLS) {
			stype = types[type];
		} else {
			switch (type) {
			case PT_GNU_EH_FRAME:
				stype = "Frame unwind information";
				break;
			case PT_GNU_STACK:
				stype = "Stack flags";
				break;
			case PT_GNU_RELRO:
				stype = "Read-only after relocation";
				break;
			case PT_GNU_PROPERTY:
				stype = "GNU property";
				break;
			default:
				stype = "Unknown";
				break;
			}
		}

		printf("Program header %ld\n"
		       "\tp_type: %#lx, %s\n"
		       "\tp_flags: %#lx\n"
		       "\tp_offset: %#08llx\n"
		       "\tp_vaddr: %#08llx\n"
		       "\tp_paddr: %#08llx\n"
		       "\tp_filesz: %#08llx\n"
		       "\tp_memsz: %#08llx\n"
		       "\tp_align: %#08llx\n",
		       i,
		       type, stype,
		       (long) phdr->flags,
		       (long long) phdr->offset,
		       (long long) phdr->vaddr,
		       (long long) phdr->paddr,
		       (long long) phdr->filesz,
		       (long long) phdr->memsz,
		       (long long) phdr->align);

		putchar('\t');
		printflags(&f);

		seg = &map->seg[i];
		printf("\tSections:");
		for (j = 0; j < seg->nchild; ++j)
			printf(" %d", seg->child[j]->sec.index);
		putchar('\n');
		putchar('\n');

		++phdr;
	}

	puts("Section to Segment mapping:\nSegment\tSections");
	for (i = 0; i < n; ++i) {
		printf(" %02lu\t", i);

		seg = &map->seg[i];
		for (j = 0; j < seg->nchild; ++j)
			printf(" %s", seg->child[j]->sec.name);
		putchar('\n');
	}
	putchar('\n');

	delmap(map);
}

void
elffhdr(Obj *obj, unsigned long long *start, Flags *f)
{
	size_t i;
	char *name;
	Elf *elf = obj->data;
	Elfhdr *hdr = &elf->hdr;
	Elfsec *shdr;

	*start = hdr->entry;

	for (i = 0; i < elf->nsec; i++) {
		shdr = &elf->secs[i];
		name = shdr->name;
		setflag(f, strncmp(name, ".debug", 6) == 0, HAS_DEBUG);
		setflag(f, strncmp(name, ".line", 5) == 0, HAS_LINENO);
		setflag(f, strcmp(name, ".debug_line") == 0, HAS_LINENO);
		setflag(f, shdr->type == SHT_RELA, HAS_RELOC);
		setflag(f, shdr->type == SHT_REL, HAS_RELOC);
	}

	setflag(f, hdr->type == ET_EXEC, EXEC_P);
	setflag(f, hdr->type == ET_DYN, DYNAMIC);
	setflag(f, elf->nsym > 0, HAS_SYMS);

	if (!pflag)
		return;

	printfhdr(hdr);
	printphdr(obj, elf->phdr, hdr->phnum);
}
