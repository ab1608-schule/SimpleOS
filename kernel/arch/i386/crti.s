.section .init
.global _init
.type _init, @function
_init:
	push %ebp
	movl %esp, %ebp
	// Automatically filled by GCC

.section .fini
.global _fini
.type _fini, @function
_fini:
	push %ebp
	movl %esp, %ebp
	// Automatically filled by GCC
