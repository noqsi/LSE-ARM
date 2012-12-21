/* not just parameters anymore */

struct usart_parameters {
	struct usart *usart;	/* pointer to the hardware device */
	unsigned baud;		/* Baud rate */
	unsigned flags;		/* flags to/from driver */
	char intr_char;		/* character for user interrupt */
	char rxchar;		/* last received character */
};

/* Flag definitions */

#define UF_CR	1		/* For serial controllers that end line with CR */
#define UF_BREAK 2		/* Enable interrupt on break (framing error) */
#define UF_INTR 4		/* Enable interrupt on intr_char */
#define UF_RXRDY 8		/* Have a character */
	
extern void usart_init( struct usart_parameters *p, int n );
extern char usart_getc( int un );
extern void usart_putc( int un, char c );
extern void usart_interrupt( int un );
extern int usart_rxrdy( int un );

/*
 * $Log$
 * Revision 1.3  2010-06-10 17:53:07  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
 *
 * Revision 1.2  2010-06-08 18:57:41  jpd
 * Faults and user interrupts now work on SAM7A3
 *
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.2  2010-06-05 17:32:45  jpd
 * More reorganization.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */
