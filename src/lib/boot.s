@ $Id$
@
@ Startup for C code on SAM7X
@

@ The model for this version is that we run applications in supervisor mode.
@ We use IRQ, but not FIQ.
@ Faults go to application interrupt() entry point.

.section .text
.code 32

	b boot
	b undefined
	b swi
	b abort
	b abort
	b unknown
	b irq
	b fiq

.globl boot

boot:
	ldr r13,stack_pointer
	bl	irq_setup

			@ The rest of startup is handled in C
	bl C_startup

			@ Loop forever if main() exits
	b boot

undefined:
	mov	r0,#2	@ passed to interrupt()
	b	superfault
swi:
	mov	r0,#3
	b	superfault
abort:
	mov	r0,#4
	b	superfault
unknown:
	mov	r0,#5
	b	superfault
fiq:
	mov	r0,#7
	b	superfault

.globl	irq_dispatch

irq:
	sub	r14,r14,#4		@ adjust lr
	stmfd	r13!,{r0-r3,r12,r14}	@ save context
	bl	irq_dispatch		@ do the rest in C
	ldmfd	r13!,{r0-r3,r12,pc}^	@ restore and return

@ superfault cleans up the processor state and then invokes lse's interrupt().
@ interrupt() will then attempt to restore sanity via longjmp() to the listener
@ startup. r0 should contain the token to be returned by setjmp(), which determines
@ message the user gets when this occurs.
	
.globl superfault
.globl interrupt	@ interrupt( int n ) in lse-arm.c

superfault:
	msr	cpsr_c,#0b11010011	@ force supervisor mode, interrupts masked
	bl	irq_setup		@ clear the irq stack, enable IRQ
	b	interrupt

@ Set up interrupt stack, enable IRQ.
@ Must enter in supervisor mode.
@ uses r1

.globl	irq_stack_top		@ provided by main or library

irq_setup:
	msr	cpsr_c,#0b11010010	@ force irq mode, interrupts masked
	ldr	r1,irq_stack		@ addr of constant
	ldr	r13,[r1]		@ fetch the constant
	msr	cpsr_c,#0b01010011	@ back to supervisor mode, IRQ unmasked
	mov	pc,lr
		

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Miscellaneous constants
stack_pointer:		.word _stack_top
irq_stack:		.word irq_stack_top

			.end
@
@ $Log$
@ Revision 1.2  2010-06-08 18:57:41  jpd
@ Faults and user interrupts now work on SAM7A3
@
@ Revision 1.1  2010-06-07 00:39:01  jpd
@ Massive reorganization of source tree.
@
@ Revision 1.2  2009-03-23 02:42:04  jpd
@ Version for TESS hardware.
@
@ Revision 1.1  2009-03-14 22:58:06  jpd
@ Can now run LSE in an ARM SAM7X!
@
