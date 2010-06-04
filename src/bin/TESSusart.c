/* $Id$ */

/* 
Imported from SXI, which used an older device than the SAM7X,
so it could use some updating.
*/

// Tess only runs on the 7x256 card... ~Matt
#include "memory_7x256.h"
#include "usart.h"
#include "usart_driver.h"

static struct usart *ulist[] = { USART0, USART1 };


/*
 * Init USART using constants cribbed from Angel.
 * Should make this more flexible sometime.
 * Use brgr 192 for 9600 on an 29.4912 MHz clock.
 */

void usart_init( int un ) {
	struct usart *u = ulist[un];
	
	u->cr = RSTRX | RSTTX;	/* reset RX/TX */
	u->mr = 0x8c0;	/* 8 bits, no parity */
	u->brgr = 192;	/* 9600 baud */
	u->cr = RXEN | TXEN;	/* enable RX/TX */
}

/*
 * Synchronous read and write.
 */

char usart_getc( int un )
{
	struct usart *u = ulist[un];
	char c;

	while( !(u->csr & RXRDY )) ;		/* Spin */
	c = u->rhr;
	if( c == '\r' ) c = '\n';		/* EDT ends lines with \r, but LSE uses \n */
	return c;
}

void usart_putc( int un, char c )
{
	struct usart *u = ulist[un];
	
	if( c == '\n' ) c = '\r';		/* EDT ends lines with \r, but LSE uses \n */

	while( !(u->csr & TXRDY )) ;		/* Spin */
	u->thr = c;
}


/*
 * $Log$
 * Revision 1.3  2010-06-04 18:13:17  jpd
 * Update build for multiple targets.
 *
 * Revision 1.2  2009-09-13 01:09:09  jpd
 * Exchange \n and \r in serial I/O.
 *
 * Revision 1.1  2009-03-23 02:42:04  jpd
 * Version for TESS hardware.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */
