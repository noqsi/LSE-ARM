@ $Id$
@
@ Startup for C code on SAM7X
@

@ this version reboots on any exception or interrupt

.section .text
.code 32

	b boot
	b boot
	b boot
	b boot
	b boot
	b boot
	b boot
	b boot

.globl boot

boot:
			ldr r13,stack_pointer

			@ The rest of startup is handled in C
			bl C_startup

			@ Loop forever if main() exits
			b boot


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Miscellaneous constants
stack_pointer:		.word _stack_top

			.end
@
@ $Log$
@ Revision 1.1  2009-03-14 22:58:06  jpd
@ Can now run LSE in an ARM SAM7X!
@
