/* $Id$ */

/*
 * Start up LSE on the ARM.
 */

#include "memory_7x256.h"
#include "pio.h"
#include "wdt.h"
#include "pmc.h"
#include "rstc.h"
#include "peripheral_id_7x256.h"
#include "usart_driver.h"

void lse_init( void );
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

Suitable oscillator setup for AT91SAM7X-EK eval card. The main oscillator is
18.432 MHz. Use 0x40 for OSCOUNT because that's what the ROM code seems to use,
and I have no better info.

*/

	PMC->mor = MOSCEN | OSCOUNT(0x40);

	while( !(PMC->sr & MOSCS));	/* wait for main osc stability */

/*
Select the main clock, straight through. If you want something else
you'll need to fiddle with PRES and/or the PLL.
*/

	PMC->mckr = CSS_MAIN;

/*
Enable reset via NRST pin.
*/

	RSTC->mr = RSTC_KEY | URSTEN;

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
	PIOB->oer= 0x780000;	/* enable LED's */

}


/*
 * List the usarts for the driver
 */

#define USARTS 1	/* only need one of them */

struct usart_parameters usart_list[USARTS];

/*
Finish I/O initialization and start up LSE.
Note that usart_init() has to be here, not in app_configure,
because it uses a static table, and a static variable in the driver.
*/

void app_main()
{
	usart_list[0].usart = USART0;
	usart_list[0].brgr = 10;	/* 115200 baud for 18.432 MHz clock */
	usart_init( usart_list, USARTS ); 
	lse_init();
	/* build application primitives here */
	lse_main();
}

/*
 * $Log$
 * Revision 1.1  2010-06-05 17:41:52  jpd
 * More reorganization
 *
 * Revision 1.3  2009-06-01 16:54:19  jpd
 * Installation instructions.
 * Fix line editing, allow external reset.
 *
 * Revision 1.2  2009-03-26 01:26:22  jpd
 * Better factoring.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */

