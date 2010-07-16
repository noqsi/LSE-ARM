// $Id$
//
// Pseudo file containing the readout "application"
//

.section .rodata
.globl	app_lse
app_lse :

.incbin "readout_lse.txt"

// End with a null byte
.byte	0

// $Log$
// Revision 1.1  2010-07-16 17:28:10  jpd
// Re-commit after CVS confusion.
//
