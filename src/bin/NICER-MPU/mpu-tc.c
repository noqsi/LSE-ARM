#include "tc.h"
#include "mpu-tc.h"
#include "memory_7a3.h"
#include "peripheral_id_7a3.h"
#include "lse-arm.h"
#include "aic_driver.h"

#define PPSTC (TC678->channel[2])

static volatile uint32_t ticks;

static uint16_t pps16ls;
static uint32_t pps32ms;

uint32_t rawtime( uint16_t * raw16ms )
{
	uint16_t jiffies;
	uint32_t tick1, tick2;
	
	do {			/* avoid race by demanding stable reading */
		tick1 = ticks;
		jiffies = PPSTC.cv;
		tick2 = ticks;
	} while( tick1 != tick2 );
	
	if( raw16ms ) *raw16ms = tick1 >> 16;
	
	return ( tick1 << 16 ) | jiffies;
}



static void pps_isr( void )
{
	uint32_t status = PPSTC.sr;	/* grab only once per interrupt! */
	
	if( status & COVFS ) ticks += 1;	/* counter overflow */
		 
	if( status & LDRAS ) {	/* Grab the PPS time from RA. */
	
		pps16ls = PPSTC.ra;
		pps32ms = ticks;
		
		/* 
		 * Now correct for a potential race condition.
		 * If COVFS was set, we *just* incremented "ticks",
		 * but since we don't know the order of the RA load
		 * relative to the clock overflow, it may be that
		 * the MSB's we have now are one too high for the
		 * PPS stamp. But if RA was loaded 
		 * after the overflow, we have the right MSB's, and the
		 * LSB's should then be a small number.
		 * So, we check for big numbers: a big number tells
		 * us RA was loaded before the clock overflow.
		 * This should work fine as long as the latency of 
		 * this ISR is <30000 MCLK cycles.
		 */
		 
		if( (status & COVFS) && (pps16ls > 0x8000) )
			pps32ms -= 1;
	}
}
		

void pps_start( void )
{
	ticks = 0;
	pps32ms = 0xffffffff;		/* An unlikely value */
	PPSTC.ccr = CLKEN | SWTRG;	/* reset counter, enable clock */
	PPSTC.cmr = LDRA_RISING;	/* should select burst mode here */
	PPSTC.ier = COVFS | LDRAS;	/* interrupt on the events we need */
	AIC->svr[ TC8_ID ] = (uint32_t) pps_isr;
	AIC->iecr = bit( TC8_ID );	/* turn on interrups from PPS */
}


static void rt( void ) 			/* raw time for LSE */
{ 
	uint16_t ms;
	cell ls;
	
	ls = rawtime( &ms );
	*--sp = ms;
	*--sp = ls;
}


void tc_primitives( void )
{
	build_primitive( rt, "rt" );
}
