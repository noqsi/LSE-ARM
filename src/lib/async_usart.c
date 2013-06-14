#include "usart.h"
#include "async_usart.h"
#include "aic_driver.h"

extern const unsigned mck_hz;	/* Master clock */

/*
 * Init USART. Note that the caller should explicitly set up the async_usart
 * structure, including initializing the cbuf substructures, before calling this.
 */

static struct async_usart *up;

void async_usart_init( struct async_usart *p, int n ) {
	int i;
	unsigned bck = mck_hz/16;	/* Baud rate generator clock */
	
	up = p;				/* remember where these are */
		
	for( i = 0; i < n; i += 1 ) {
		struct usart *u = p[i].usart;
	
		u->cr = RSTRX | RSTTX;	/* reset RX/TX */
		u->mr = 0x8c0;	/* 8 bits, no parity */
		/* assume simplest (DBGU) brgr variant. */
		u->brgr = (bck + p[i].baud/2)/p[i].baud;	/* set baud, round to nearest */
		u->cr = RXEN | TXEN;	/* enable RX/TX */
		u->ier = RXRDY;		/* only RX interrupt until we have TX data in q */
		if( p[i].flags & UF_BREAK ) u->ier = FRAME | RXBRK;	/* enable user interrupt */
	}
}

/*
Note: different variants show different behavior. The SAM7A3 DBGU lacks the RXBRK feature:
instead, a break causes a FRAME error. So, in the above we consider either as a break.
*/

/*
 * Blocking single character read and write.
 */

char async_usart_getc( int un )
{
	struct async_usart *u = up+un;
	int c;
	char cc;

	while(( c = cbuf_get(u->rxbuf )) == CBUF_FAIL) ;	/* Spin */
	
	cc = (char) c;
	if( (cc == '\r') && (u->flags & UF_CR)) cc = '\n';
	return cc;
}

void async_usart_putc( int un, char c )
{
	struct async_usart *u = up+un;

	if( (c == '\n') && (u->flags & UF_CR)) c = '\r';

	while( cbuf_put( u->txbuf, c ) == CBUF_FAIL ) ;		/* Spin */
	
	u->usart->ier = TXRDY;						/* Start */
}

/*
 * Non-blocking bulk read/write.
 */

int async_usart_read( int un, char *b, int n )
{
	struct async_usart *u = up+un;
	int i;
	
	for( i = 0; i < n; i += 1 ) {
		int c = cbuf_get( u->rxbuf );
		if( c == CBUF_FAIL ) break;
		*b++ = (char) c;
	}
	
	return i;
}

int async_usart_write( int un, char *b, int n )
{
	struct async_usart *u = up+un;
	int i;
	
	for( i = 0; i < n; i += 1 ) {
		if( cbuf_put( u->txbuf, *b ) == CBUF_FAIL ) break;
		b += 1;
	}
	
	if( i > 0 ) u->usart->ier = TXRDY;		/* Start */
	
	return i;
}

/*
 * Interrupt handler
 *
 * The code that sets up the interrupt vectoring should provide
 * glue that calls this with the correct unit number,
 * and if AIC is used, does the EOICR thing.
 */

extern void user_interrupt( void );	/* Called for break or interrupt char */

void async_usart_interrupt( int un )
{
	struct usart *u = up[un].usart;
	
	if( u->csr & (RXBRK|FRAME)) {	/* either might indicate a break */
		u->cr = RSTRX | RSTSTA;
		u->cr = RXEN;
		cbuf_clear( up[un].rxbuf );	/* clear any existing input */
		user_interrupt();
	}
		
	if( u->csr & RXRDY ) {		/* have a character */
		char c =  u->rhr;
		if( (up[un].flags & UF_INTR) && ( c == up[un].intr_char ))
			user_interrupt();
		else {
			(void) cbuf_put( up[un].rxbuf, c);	/* ignore errors for now */
		}
	}

	if( u->csr & TXRDY ) {		/* have a character */
		int c =  cbuf_get( up[un].txbuf );
		
		if( c == CBUF_FAIL ) u->idr = TXRDY;		/* Empty, stop output */
		else u->thr = c;
	}
}


