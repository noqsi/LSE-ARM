/* $Id$ */

/* 
Imported from SXI, which used an older device than the SAM7X,
so it could use some updating.
*/

// #include "memory.h"
#include "usart.h"
#include "usart_driver.h"
#include "aic_driver.h"

/*
 * Init USART using constants cribbed from Angel.
 * Should make this more flexible sometime.
 */

static struct usart_parameters *up;

void usart_init( struct usart_parameters *p, int n ) {
	int i;
	
	up = p;
	
	for( i = 0; i < n; i += 1 ) {
		struct usart *u = p[i].usart;
	
		u->cr = RSTRX | RSTTX;	/* reset RX/TX */
		u->mr = 0x8c0;	/* 8 bits, no parity */
		u->brgr = p[i].brgr;	/* set baud */
		u->cr = RXEN | TXEN;	/* enable RX/TX */
		if( p[i].flags & UF_BREAK ) u->ier = FRAME | RXBRK;	/* enable user interrupt */
	}
}

/*
Note: different variants show different behavior. The SAM7A3 DBGU lacks the RXBRK feature:
instead, a break causes a FRAME error. So, in the above we consider either as a break.
*/

/*
 * Synchronous read and write.
 */

char usart_getc( int un )
{
	struct usart *u = up[un].usart;
	char c;

	while( !(u->csr & RXRDY )) ;		/* Spin */
	c = u->rhr;

	if( (c == '\r') && (up[un].flags & UF_CR)) c = '\n';
	return c;
}

void usart_putc( int un, char c )
{
	struct usart *u = up[un].usart;

	if( (c == '\n') && (up[un].flags & UF_CR)) c = '\r';

	while( !(u->csr & TXRDY )) ;		/* Spin */
	u->thr = c;
}


/*
 * Interrupt handler
 *
 * The code that sets up the interrupt vectoring should provide
 * glue that calls this with the correct unit number,
 * and if AIC is used, does the EOICR thing.
 */

extern void user_interrupt( void );

void usart_interrupt( int un )
{
	struct usart *u = up[un].usart;
	u->cr = RSTRX | RSTSTA;
	u->cr = RXEN;
	user_interrupt();
}

/*
 * $Log$
 * Revision 1.3  2010-06-08 20:25:38  jpd
 * Interrupts working with SAM7X256 board, too.
 *
 * Revision 1.2  2010-06-08 18:57:41  jpd
 * Faults and user interrupts now work on SAM7A3
 *
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.3  2010-06-05 17:32:45  jpd
 * More reorganization.
 *
 * Revision 1.2  2009-06-01 16:54:19  jpd
 * Installation instructions.
 * Fix line editing, allow external reset.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */

