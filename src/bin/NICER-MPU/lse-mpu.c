/*
 * Start up LSE on the ARM.
 */

#include "memory_7a3.h"
#include "pio.h"
#include "wdt.h"
#include "pmc.h"
#include "rstc.h"
#include "aic.h"
#include "mc.h"
#include "peripheral_id_7a3.h"
#include "usart_driver.h"
#include "aic_driver.h"
#include "pit_driver.h"
#include "mpu-tc.h"
#include "lse-arm.h"
#include "ad53bb.h"
#include "pulsar_spi.h"

void lse_init( void );
void lse_main( void );
void copy_static( void );

/*
Let everyone know what the master clock frequency is.
*/

#define MAINCLK 18432000
#define PLLDIV	24
#define PLLMUL	130
#define PRESCALE 1

const unsigned mck_hz = (MAINCLK/(PLLDIV*(1<<PRESCALE)))*PLLMUL;

/*
List the source "files" to read at initialization.
Each is a null-terminated character array made up of 
lines ending in \n.
*/

extern char init_lse[], app_lse[];
char * const init_source[] = {init_lse, app_lse, 0};

/*
A little custom feature is to blink a light at 1 hz.
Do this on the system tick.
*/

#define TICK_HZ 1000

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

Enable reset via NRST pin. It's important to do this early. Without this, if you
somehow make it catatonic by disabling the processor clock you won't be able do
anything through JTAG. If an initialization bug causes the problem, you won't be
able to halt the processor quickly enough to get it under control after power-up
or reset. But if the processor enters the catatonic state after execution of the
line below, you can escape by issuing "jtag_reset 1 1; halt" followed by
"jtag_reset 0 0" in OpenOCD. This will halt the processor, and then you can
reflash it to fix the bug. 

*/

	RSTC->mr = RSTC_KEY | URSTEN;

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

When we crank up the clock, we'll need wait states on flash.

*/

	MC->fmr = FWAIT2;

/*

Program the PLL

*/

	PMC->pllr = DIV(PLLDIV) | PLLCOUNT(63) | MUL(PLLMUL);
	
	while( !(PMC->sr & LOCK));	/* wait for PLL stability */
	
/*

Prescale if necessary.

*/

#if PRESCALE

	PMC->mckr = PRES(PRESCALE);
	
	while( !(PMC->sr & MCKRDY));	/* wait for prescaler */
	
#endif
	

/*
Select the PLL clock.
*/

	PMC->mckr |= CSS_PLL;

/*
Turn on peripheral clocks, as needed.
*/

        PMC->pcer = 0xffffffff;         /* for development, turn 'em all on */


/*
Configure IO pins here. Other peripheral configuration can happen in drivers,
but it's useful to gather the physical pin configuration together in one place,
for clarity and to avoid conflict.
*/

	PIOA->asr = 0xc003f800;	/* enable TXD0, RXD0, SPI0 */
	PIOA->pdr = 0xc0038000;	/* relinquish pins to USART0, SPI0 */
	PIOA->oer = 0x03300000;	/* enable LED's */
	PIOA->sodr = 0x03300000;	/* turn off LED's */
	PIOA->oer = 0x00000610;		/* bitbang DAC */
	PIOA->oer = 0x00007800;		/* bitbang SPI0 select bits */
	PIOA->sodr = 0x00007800;	/* deselect */
	PIOA->oer = 0x20000000;		/* Use TCLK6 to gate TC */
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
struct spi *pulsar_spi = SPI0;
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
	pulsar_spi_primitives();
	pulsar_spi_init( SPI0 );
	ad53_primitives();
	tc_start();	/* go live on raw clock and PPS */
	lse_main();
}


