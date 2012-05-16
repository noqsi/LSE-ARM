//
// Pseudo file containing the git date stamp and hash
//

.section .rodata
.globl	stamp
stamp :

.incbin "stamp.txt"

// End with a null byte
.byte	0
