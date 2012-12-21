#ifndef ASYNC_USART_H
#define ASYNC_USART_H

#include "cbuf.h"

struct async_usart {
	struct usart *usart;	/* pointer to the hardware device */
	unsigned baud;		/* Baud rate */
	unsigned flags;		/* flags to/from driver */
	char intr_char;		/* character for user interrupt */
	struct cbuf *rxbuf;	/* receive buffer */
	struct cbuf *txbuf;	/* transmit buffer */
};

/* Flag definitions */

#define UF_CR	1		/* For serial controllers that end line with CR */
#define UF_BREAK 2		/* Enable interrupt on break (framing error) */
#define UF_INTR 4		/* Enable interrupt on intr_char */
	
extern void async_usart_init( struct async_usart *p, int n );
extern char async_usart_getc( int un );
extern void async_usart_putc( int un, char c );
extern int async_usart_read( int un, char *b, int n );
extern int async_usart_write( int un, char *b, int n );
extern void async_usart_interrupt( int un );

#endif /* ndef ASYNC_USART_H */
