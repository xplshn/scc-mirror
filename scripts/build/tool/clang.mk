include $(BUILDDIR)/tool/gnu.mk

NOCARET = -fno-caret-diagnostics
TOOL_ASFLAGS = -c

COMP = clang
ASM = clang
LINKER = ld.lld
OBJCOPY = llvm-objcopy
OBJDUMP = llvm-objdump
