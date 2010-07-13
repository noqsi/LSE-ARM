/* $Id$ */
/* 
Imported from SXI, which used an older device than the SAM7X,
so it could use some updating.
*/

#ifndef bit
#define bit(n) (1<<(n))
#endif

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
#define TXDIS	bit(7)
#define RSTSTA	bit(8)

/*
 * Status/interrupt bits
 */

#define	RXRDY	bit(0)
#define	TXRDY	bit(1)
#define RXBRK	bit(2)		/* DBGU lacks this */
#define	ENDRX	bit(3)
#define ENDTX	bit(4)
#define OVRE	bit(5)
#define FRAME	bit(6)
#define PARE	bit(7)
#define TXEMPTY	bit(9)
#define TXBUFE	bit(11)
#define RXBUFF	bit(12)


/*
 * $Log$
 * Revision 1.4  2010-07-13 18:38:07  jpd
 * First draft of low level SPI driver.
 *
 * Revision 1.3  2010-06-08 20:25:38  jpd
 * Interrupts working with SAM7X256 board, too.
 *
 * Revision 1.2  2010-06-08 18:57:41  jpd
 * Faults and user interrupts now work on SAM7A3
 *
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */
