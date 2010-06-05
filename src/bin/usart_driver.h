/* $Id$ */

struct usart_parameters {
	struct usart *usart;	/* pointer to the hardware device */
	unsigned brgr;		/* Baud rate generator register */
};
	
extern void usart_init( struct usart_parameters *p, int n );
extern char usart_getc( int un );
extern void usart_putc( int un, char c );

/*
 * $Log$
 * Revision 1.2  2010-06-05 17:32:45  jpd
 * More reorganization.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */
