/* $Id$ */

/*
 * Start up LSE on the ARM.
 */

#include "memory_7x256.h"
#include "pio.h"
#include "wdt.h"
#include "pmc.h"
#include "rstc.h"
#include "aic.h"
#include "peripheral_id_7x256.h"
#include "usart_driver.h"
#include "aic_driver.h"

void lse_init( void );
void lse_main( void );
void copy_static( void );
void SeqPrimitives( void );

/*
Let everyone know what the master clock frequency is.
*/

#ifdef EKBOARD	/* Atmel dev board, not the real TESS hardware */

const unsigned mck_hz = 18432000;

#else /* TESS hardware uses a faster oscillator */

const unsigned mck_hz = 29491200;

#endif

/*
Provide I/O primitives.
*/

char readchar( void ){ return usart_getc( 0 ); }

void writechar( char c ){ usart_putc( 0, c ); }

/*
Provide glue for USART ISR
*/

static void usart0_interrupt( void )
{
	usart_interrupt( 0 );
	AIC->eoicr = 0;		/* acknowledge the interrupt to AIC */
}



/*
 * Configure processor to run the application.
 * Do not use any static variables in this function!
 */
 
void app_configure( void )
{

/* 

If you want to actually use the watchdog, you'll have to change this, as you
get no second chance short of a processor reset! 

*/

	watchdog_disable();
	
#ifdef EKBOARD	/* Atmel dev board, not the real TESS hardware */

/* 

Suitable oscillator setup for AT91SAM7X-EK eval card. The main oscillator is
18.432 MHz. Use 0x40 for OSCOUNT because that's what the ROM code seems to use,
and I have no better info.

*/

	PMC->mor = MOSCEN | OSCOUNT(0x40);

	while( !(PMC->sr & MOSCS));	/* wait for main osc stability */

#else

/* 

Suitable oscillator setup for the TESS Interface board. The external oscillator
is 29.4912 MHz.

*/

	PMC->mor = OSCBYPASS;

#endif

/*
Select the main clock, straight through. If you want something else
you'll need to fiddle with PRES and/or the PLL.
*/

	PMC->mckr = CSS_MAIN;

/*
Turn on peripheral clocks, as needed.
*/

	PMC->pcer = bit(US0_ID);	/* USART0 */

/*
Configure IO pins here. Other peripheral configuration can happen in drivers,
but it's useful to gather the physical pin configuration together in one place,
for clarity and to avoid conflict.
*/

	PIOA->asr = 0x3;	/* enable TXD0, RXD0 */
	PIOA->pdr = 0x3;	/* relinquish pins to USART0 */
	PIOA->oer = 0x2;	/* enable output on TXD0 */
	
//	PIOB->oer = 0x780000;	/* eval kit LED's */
//	PIOB->sodr = 0x500000;	
//	PIOB->codr = 0x280000;
	
//	for(;;); /* debug */

/*
Set up to dispatch interrupts to the error handler, so that once we start turning
on peripherals, any interrupt anomaly will attempt to produce a message.
*/

	irq_dispatch_init();

/*
Now set up USART interrupt
*/

	AIC->svr[ US0_ID ] = (uint32_t) usart0_interrupt;
	AIC->iecr = bit( US0_ID );

}

/*
 * List the usarts for the driver
 */

#define USARTS 1	/* only need one of them */

struct usart_parameters usart_list[USARTS];


/*
Finish I/O initialization and start up LSE.
Note that usart_init() has to be here, not in app_configure,
because it uses a static table.
*/


void app_main()
{
	copy_static();			/* Need to do this before I/O init */
	usart_list[0].usart = USART0;
	usart_list[0].baud = 115200;
	usart_list[0].flags = UF_CR	/* CR is end of line */
			| UF_BREAK	/* Interrupt on break from terminal */
			| UF_INTR;	/* Interrupt on special char */
	usart_list[0].intr_char = 2;	/* ^B */
	usart_init( usart_list, USARTS );
	lse_init();
	SeqPrimitives();	/* setup the seq words */
	lse_main();
}

/*
 * $Log$
 * Revision 1.4  2010-09-06 20:33:57  jpd
 * Raise baud to 115200
 *
 * Revision 1.3  2010-06-10 17:53:06  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
 *
 * Revision 1.2  2010-06-08 20:59:57  jpd
 * Update for interrupts.
 *
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.4  2010-06-04 18:13:17  jpd
 * Update build for multiple targets.
 *
 * Revision 1.3  2009-03-26 02:26:04  jpd
 * Init seq words.
 * Fix ambiguous stack op.
 *
 * Revision 1.2  2009-03-26 01:26:22  jpd
 * Better factoring.
 *
 * Revision 1.1  2009-03-23 02:42:04  jpd
 * Version for TESS hardware.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */

