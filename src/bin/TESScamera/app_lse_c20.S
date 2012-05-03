// $Id$
//
// Pseudo file containing the "application"
//

.section .rodata
.globl	app_lse
app_lse :

.incbin "c20.txt"

// End with a null byte
.byte	0

// $Log$
