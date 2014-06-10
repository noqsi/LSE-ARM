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

#define MAINCLK 10000000	/* 10 MHz */
#define PLLDIV	1
#define PLLMUL	12
#define PRESCALE 2

const unsigned mck_hz = (MAINCLK/(PLLDIV*(1<<PRESCALE)))*PLLMUL;

/*
List the source "files" to read at initialization.
Each is a null-terminated character array made up of 
lines ending in \n.
*/

extern char init_lse[], app_lse[], stamp[];
char * const init_source[] = {init_lse, app_lse, stamp, "1 doPrompt !\n", 0};

/*
Provide I/O primitives.
*/

char readchar( void ){ return usart_getc( 0 ); }

void writechar( char c ){ usart_putc( 0, c ); }

int char_ready( void ) { return usart_rxrdy( 0 ); }

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

Suitable oscillator setup for the TESS RRU. The external oscillator
is 10 MHz.

*/

	PMC->mor = OSCBYPASS;

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

	PMC->pcer = 0xffffffff;	/* All on for dev */

/*
Configure IO pins here. Other peripheral configuration can happen in drivers,
but it's useful to gather the physical pin configuration together in one place,
for clarity and to avoid conflict.
*/

	PIOA->asr = 0x3;	/* enable TXD0, RXD0 */
	PIOA->bsr = 0x49e00000; /* programmable clocks, SPI1 */
	PIOA->pdr = 0x49e00003;	/* relinquish pins to peripherals */
	PIOA->sodr = 0x2006fffc; /* initialize bitbang outputs to 1 */
	PIOA->oer = 0x2006fffc;	/* enable bitbang outputs */
	PIOB->oer = 0x04fc0000;	/* enable address lines on Port B */

	PIOA->pudr = 0x7fffffff;	/* get rid of pull up resistors on Port A */
	PIOB->pudr = 0x7fffffff;	/* get rid of pull up resistors on Port B */	

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

