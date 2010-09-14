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
#include "pit_driver.h"
#include "spi_driver.h"
#include "lse-arm.h"

void lse_init( void );
void lse_main( void );
void copy_static( void );
void spi_primitives( void );

/*
Let everyone know what the master clock frequency is.
*/

const unsigned mck_hz = 18432000;


/*
A little custom feature is to blink a light at 1 hz.
Do this on the system tick.
*/

#define TICK_HZ 1000

static void blink( void )
{
	static unsigned count;
	if( count < TICK_HZ/2 )
		PIOB->sodr = 0x400000;
	else
		PIOB->codr = 0x400000;
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
The interval timer is part of the system controller, so that's the interrupt
it needs to service. Turn this into a dispatcher if you need more SYSC interrupts.
*/

static void sysc_isr( void )
{
	pit_isr();
	AIC->eoicr = 0;		/* acknowledge the interrupt to AIC */
}

/*
Provide glue for USART ISR
*/

static void usart0_interrupt( void )
{
	usart_interrupt( 0 );
	AIC->eoicr = 0;		/* acknowledge the interrupt to AIC */
}

/*
Provide glue for SPI ISRs
*/

static void spi0_interrupt( void )
{
	spi_isr( 0 );
	AIC->eoicr = 0;		/* acknowledge the interrupt to AIC */
}


static void spi1_interrupt( void )
{
	spi_isr( 1 );
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

	PMC->pcer = bit(US0_ID);	/* USART0 */
	PMC->pcer = bit(SPI0_ID);
	PMC->pcer = bit(SPI1_ID);

/*
Configure IO pins here. Other peripheral configuration can happen in drivers,
but it's useful to gather the physical pin configuration together in one place,
for clarity and to avoid conflict.
*/

	PIOA->asr = 0x3;	/* enable TXD0, RXD0 */
	PIOA->pdr = 0x3;	/* relinquish pins to USART0 */
	PIOA->asr = 0x70000;	/* enable SPI0 data, clock */
	PIOA->pdr = 0x70000;
	PIOA->bsr = 0x27e00380;	/* enable SPIO CS, SPI1 */
	PIOA->pdr = 0x27e00380;
	PIOB->oer= 0x780000;	/* enable LED's */
	PIOB->sodr= 0x780000;	/* turn them off */
	
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

	AIC->svr[ US0_ID ] = (uint32_t) usart0_interrupt;
	AIC->iecr = bit( US0_ID );

	AIC->svr[ SPI0_ID ] = (uint32_t) spi0_interrupt;
	AIC->iecr = bit( SPI0_ID );

	AIC->svr[ SPI1_ID ] = (uint32_t) spi1_interrupt;
	AIC->iecr = bit( SPI1_ID );

}


/*
 * List the usarts for the driver
 */

#define USARTS 1	/* only need one of them */

struct usart_parameters usart_list[USARTS];

/* SPI */

#define SPIS 2
struct spi_parameters spi_list[SPIS];

/*
Finish I/O initialization and start up LSE.
Note that usart_init() has to be here, not in app_configure,
because it uses a static table, and a static variable in the driver.
*/

void app_main()
{
	copy_static();			/* Need to do this before I/O init */
	usart_list[0].usart = USART0;
	usart_list[0].baud = 115200;
	usart_list[0].flags = UF_BREAK;	/* Interrupt on break from terminal */
	usart_init( usart_list, USARTS );
	spi_list[0].spi = SPI0;
	spi_list[1].spi = SPI1;
	spi_init( spi_list, SPIS ); 
	init_pit( PIT, TICK_HZ );
	on_tick = blink; 
	lse_init();
	/* build application primitives here */
	build_primitive( msec, "msec" );
	spi_primitives();
	lse_main();
}

/*
 * $Log$
 * Revision 1.2  2010-09-14 00:01:36  jpd
 * SPI working in local loopback mode.
 *
 * Revision 1.1  2010-07-16 17:28:10  jpd
 * Re-commit after CVS confusion.
 *
 * Revision 1.1  2010-07-15 17:52:22  jpd
 * *** empty log message ***
 *
 * Revision 1.3  2010-06-10 17:53:07  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
 *
 * Revision 1.2  2010-06-08 20:25:38  jpd
 * Interrupts working with SAM7X256 board, too.
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

