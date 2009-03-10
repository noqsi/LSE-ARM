// $Id$
//
// Pseudo file for initializing a stand-alone LSE
//

.section .rodata
.globl	init_lse
init_lse :

// Bring in the basic definitions
.incbin "init.lse"

// End with a null byte
.byte	0

// $Log$
// Revision 1.1  2009-03-10 19:08:44  jpd
// Initial revision
//
