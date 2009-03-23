/* $Id$ */

/*
 * Start up LSE on the ARM.
 */

#include "memory.h"
#include "pio.h"
#include "wdt.h"
#include "pmc.h"
#include "peripheral_id.h"
#include "usart_driver.h"

void lse_main( void );

/*
Provide an "application". Placeholder.
*/

char app_lse[] = "1 doPrompt !\n";

/*
Provide I/O primitives.
*/

char readchar( void ){ return usart_getc( 0 ); }

void writechar( char c ){ usart_putc( 0, c ); }


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

/* 

Suitable oscillator setup for the TESS Interface board. The external oscillator
is 29.4912 MHz.

*/

	PMC->mor = OSCBYPASS;

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

}

/*
Finish I/O initialization and start up LSE.
Note that usart_init() has to be here, not in app_configure,
because it uses a static table.
*/


void app_main()
{
	usart_init( 0 );
	lse_main();
}

/*
 * $Log$
 * Revision 1.1  2009-03-23 02:42:04  jpd
 * Version for TESS hardware.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */

