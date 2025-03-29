	.file   "setjmp.s"

	.text
	.globl	setjmp
setjmp:
	// IHI0055B_aapcs64.pdf 5.1.1, 5.1.2 callee saved registers
	stp x19, x20, [x0,#0]
	stp x21, x22, [x0,#16]
	stp x23, x24, [x0,#32]
	stp x25, x26, [x0,#48]
	stp x27, x28, [x0,#64]
	stp x29, x30, [x0,#80]
	mov x2, sp
	str x2, [x0,#96]
	stp  d8,  d9, [x0,#104]
	stp d10, d11, [x0,#120]
	stp d12, d13, [x0,#136]
	stp d14, d15, [x0,#152]
	mov x0, #0
	ret
