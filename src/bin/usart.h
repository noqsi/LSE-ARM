/* $Id$ */
/* 
Imported from SXI, which used an older device than the SAM7X,
so it could use some updating.
*/


struct usart {
	volatile unsigned cr, mr, ier, idr, imr, csr, rhr, thr, brgr, rtor,
		ttgr, reserved;
};

/*
 * Status bits
 */

#define RXRDY	bit(0)
#define	TXRDY	bit(1)

/*
 * Control bits
 */

#define RSTRX	bit(2)
#define RSTTX	bit(3)
#define RXEN	bit(4)
#define TXEN	bit(6)


/*
 * $Log$
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */
