/* $Id$ */

/* 
Imported from SXI, which used an older device than the SAM7X,
so it could use some updating.
*/

#include "memory.h"
#include "usart.h"
#include "usart_driver.h"

static struct usart *ulist[] = { USART0, USART1 };


/*
 * Init USART using constants cribbed from Angel.
 * Should make this more flexible sometime.
 * Use brgr 10 for 115200 on an 18.432 MHz clock.
 */

void usart_init( int un ) {
	struct usart *u = ulist[un];
	
	u->cr = RSTRX | RSTTX;	/* reset RX/TX */
	u->mr = 0x8c0;	/* 8 bits, no parity */
	u->brgr = 10;	/* 115200 baud */
	u->cr = RXEN | TXEN;	/* enable RX/TX */
}

/*
 * Synchronous read and write.
 */

char usart_getc( int un )
{
	struct usart *u = ulist[un];

	while( !(u->csr & RXRDY )) ;		/* Spin */
	return u->rhr;
}

void usart_putc( int un, char c )
{
	struct usart *u = ulist[un];

	while( !(u->csr & TXRDY )) ;		/* Spin */
	u->thr = c;
}


/*
 * $Log$
 * Revision 1.2  2009-06-01 16:54:19  jpd
 * Installation instructions.
 * Fix line editing, allow external reset.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */

