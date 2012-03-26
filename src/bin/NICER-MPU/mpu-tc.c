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

static volatile uint32_t ticks;	/* PPS timer overflows */

static volatile uint16_t pps16ls;
static volatile uint32_t pps32ms;

static struct photons {
	cell rawstamp, atime, fast_pulse, btime, slow_pulse, tag, done;
} photons[2];

int photon;	/* id the next possibly available photon buffer */

// static cell ovcorr;

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

(void) x_isr( int n, struct tc_channel *c )
{
	uint32_t status = c->sr;
	struct photons *p = photons + photon;

	PIOB->codr = 0xf;		/* clear chain select bits */
	PIOB->sodr = n;			/* select fast chain n */
		
	if( !p->tag ) {	/* buffer available, record the event */					p->rawstamp = rawtime( 0 );	/* MPU timestamp (32) */
		p->atime = c->ra;		/* trigger time (16) */
		/* MUCH MISSING!!!! */	
		p->tag = ((PIOB->pdsr&0xf0)>>4);
		p->done = c->cv;		/* record finish time */
	}
	
	PIOB->sodr = 0x100;	/* bang on reset */
	PIOB->codr = 0x100;
}
	
		
/* vector X-ray interrupts to common handler */

(void) x0_isr( void ) { x_isr( 0, &TC012->channel[0] ); }
(void) x1_isr( void ) { x_isr( 1, &TC012->channel[1] ); }
(void) x2_isr( void ) { x_isr( 2, &TC012->channel[2] ); }
(void) x3_isr( void ) { x_isr( 3, &TC345->channel[0] ); }
(void) x4_isr( void ) { x_isr( 4, &TC345->channel[1] ); }
(void) x5_isr( void ) { x_isr( 5, &TC345->channel[2] ); }
(void) x6_isr( void ) { x_isr( 6, &TC678->channel[0] ); }
(void) x7_isr( void ) { x_isr( 7, &TC678->channel[1] ); }

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
	for(;;) {				/* loop until status clean */
		uint32_t status = PPSTC.sr;	/* grab only once per cycle! */
		
		if( !(status & 0xff)) return;
			
		if( status & COVFS ) ticks += 1;  /* counter overflow */
		 
		if( status & LDRAS ) {	/* Grab the PPS time from RA. */
	
		/* test code for checking with 80 kHz TCXO */
		
//			static int prescale = 1;
//		
//			if( --prescale ) return;  /* Haven't counted to zero */
//			prescale = 80000;
		
		/* end of test code */
	
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
		 
			if( (status & COVFS) && (pps16ls > 0x8000) ) {
				pps32ms -= 1;
//				ovcorr += 1;
			}
		}
	}
}
		

void pps_start( void )
{
	ticks = 0;
	pps32ms = 0xffffffff;		/* An unlikely value */
	GATE_PORT->codr = GATE_BIT;	/* gate bit to zero */
	PPSTC.cmr = LDRA_RISING | LDRB_FALLING | ANDXC0; 
				/* gate clock with XC0 */
	PPSTC.ier = LDRAS | LDRBS | COVFS;
				/* interrupt on the events we need */
	PPSTC.ccr = CLKEN;	/* enable clock */
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
	PPSTC.ccr = SWTRIG;		/* reset clock, allow register loads */
	GATE_PORT->sodr = GATE_BIT;
}


void tc_primitives( void )
{
	build_primitive( rt, "rt" );
	build_primitive( pps, "pps" );
	build_primitive( rt_on, "rt-on" );
//	build_named_constant( (cell) &ovcorr, "ovcorr");
}
