/* $Id$ */

/*
 * Start up LSE on the ARM.
 */

#include "memory_7a3.h"
#include "pio.h"
#include "wdt.h"
#include "pmc.h"
#include "rstc.h"
#include "aic.h"
#include "peripheral_id_7a3.h"
#include "usart_driver.h"
#include "aic_driver.h"
#include "pit_driver.h"
#include "mpu-tc.h"
#include "lse-arm.h"
#include "ad53bb.h"
#include "pulsar.h"

void lse_init( void );
void lse_main( void );
void copy_static( void );

/*
Let everyone know what the master clock frequency is.
*/

const unsigned mck_hz = 18432000;

/*
Provide an "application". Placeholder.
*/

// char app_lse[] = "\" \nNICER MPU\n\" ,t 1 doPrompt !\n";

/*
A little custom feature is to blink a light at 1 hz.
Do this on the system tick.
*/

#define TICK_HZ 4000

static void blink( void )
{
	static unsigned count;
	if( count < TICK_HZ/2 )
		PIOA->sodr = 0x00100000;
	else
		PIOA->codr = 0x00100000;
	count += 1;
	if( count >= TICK_HZ ) count = 0;

}

/*
LSE primitive to get milliseconds since boot.
*/

#define MSEC_TICKS (TICK_HZ/1000)

void msec ( void ) {*--sp = ticks/MSEC_TICKS;}

/*
Provide I/O primitives.
*/

char readchar( void ){ return usart_getc( 0 ); }

void writechar( char c ){ usart_putc( 0, c ); }

/*
We're using interrupts from two devices on the system controller, so we need
a dispatcher to figure out which ISR to invoke.
*/

static void sysc_isr( void )
{
	if( PIT->sr & PITS ) pit_isr();
	else usart_interrupt( 0 );	/* by elimination */
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

        PMC->pcer = 0xffffffff;         /* for development, turn 'em all on */


/*
Configure IO pins here. Other peripheral configuration can happen in drivers,
but it's useful to gather the physical pin configuration together in one place,
for clarity and to avoid conflict.
*/

	PIOA->asr = 0xc0000000;	/* enable TXD0, RXD0 */
	PIOA->pdr = 0xc0000000;	/* relinquish pins to USART0 */
	PIOA->oer = 0x03300000;	/* enable LED's */
	PIOA->sodr = 0x03300000;	/* turn off LED's */
	PIOA->oer = 0x00020e10;		/* bitbang ADC and DAC */
	PIOA->oer = 0x20000000;		/* Use TCLK6 to gate TC *
	PIOA->puer = bit(15);		/* set pullup on ADC data */
	PIOB->oer = 0x00000f0f;		/* enable mpu control bits */
	PIOB->asr = 0x15555000;		/* select timer inputs */
	PIOB->pdr = 0x15555000;		/* assign pins to timer */

/*
Set up to dispatch interrupts to the error handler, so that once we start turning
on peripherals, any interrupt anomaly will attempt to produce a message.
*/

	irq_dispatch_init();

/*
Now set up interrupts
*/

	AIC->svr[ SYSC_ID ] = (uint32_t) sysc_isr;
	AIC->iecr = bit( SYSC_ID );
}


/*
 * List the usarts for the driver
 */

#define USARTS 1	/* only need one of them */

struct usart_parameters usart_list[USARTS];

/*
 * ADC and DAC structures
 */

struct pulsar pulsar_adc[1];
struct ad53 ad53_dac[1];

/*
Finish I/O initialization and start up LSE.
Note that device driver initializations generally have to be here, not in app_configure,
because static variables are generally involved.
*/

void app_main()
{
	copy_static();			/* Need to do this before I/O init */
	usart_list[0].usart = DBGU;	/* Use the debug unit for serial IO */
	usart_list[0].baud = 115200;
	usart_list[0].flags = 
		UF_BREAK |		/* Interrupt on break from terminal */
		UF_CR;			/* Allow CR as end of line */
	usart_init( usart_list, USARTS );
	pulsar_adc[0].pio = PIOA;
	pulsar_adc[0].sck = bit(17);
	pulsar_adc[0].cnv = bit(11);
	pulsar_adc[0].sdo = bit(15);
	pulsar_adc[0].bits = 18;
	pulsar_adc[0].snx = 0xfffe0000;
	ad53_dac[0].pio = PIOA;
	ad53_dac[0].sclk = bit(10);
	ad53_dac[0].sync = bit(4);
	ad53_dac[0].din = bit(9);
	init_pit( PIT, TICK_HZ );
	on_tick = blink; 
	lse_init();
	/* build application primitives here */
	build_primitive( msec, "msec" );
	tc_primitives();
	pulsar_primitives();
	ad53_primitives();
	pps_start();	/* go live on raw clock and PPS */
	lse_main();
}

/*
 * $Log$
 * Revision 1.4  2010-07-13 18:38:05  jpd
 * First draft of low level SPI driver.
 *
 * Revision 1.3  2010-06-10 17:53:06  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
 *
 * Revision 1.2  2010-06-08 18:56:55  jpd
 * Faults and user interrupts now work on SAM7A3
 *
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
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

