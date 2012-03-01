#include "tc.h"
#include "mpu-tc.h"
#include "memory_7a3.h"
#include "peripheral_id_7a3.h"
#include "lse-arm.h"
#include "aic.h"
#include "pio.h"

#define PPSTC (TC678->channel[2])
#define GATE_BIT 0x20000000
#define GATE_PORT (PIOA)

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

uint32_t last_pps( uint16_t * pps16ms )
{
	uint16_t ls;
	uint32_t ms1, ms2;
	
	do {			/* avoid race by demanding stable reading */
		ms1 = pps32ms;
		ls = pps16ls;
		ms2 = pps32ms;
	} while( ms1 != ms2 );
	
	if( pps16ms ) *pps16ms = ms1 >> 16;
	
	return ( ms1 << 16 ) | ls;
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
	GATE_PORT->codr = GATE_BIT;	/* gate bit to zero */
	PPSTC.cmr = LDRA_RISING | ANDXC0; /* gate clock with XC0 */
	PPSTC.ier = COVFS | LDRAS;	/* interrupt on the events we need */
	PPSTC.ccr = CLKEN | SWTRIG;	/* reset counter, enable clock */
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


static void pps( void ) 		/* pps time for LSE */
{ 
	uint16_t ms;
	cell ls;
	
	ls = last_pps( &ms );
	*--sp = ms;
	*--sp = ls;
}

static void rt_on( void )
{
	GATE_PORT->sodr = GATE_BIT;
}


void tc_primitives( void )
{
	build_primitive( rt, "rt" );
	build_primitive( pps, "pps" );
	build_primitive( rt_on, "rt-on" );
}
