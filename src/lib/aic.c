/* $Id$ */

#include "aic.h"
#include "aic_driver.h"

/*
 * Mostly, individual drivers can do what they need by talking
 * directly to the AIC. Only initialization, dispatch, and error
 * recovery are handled centrally here.
 */

extern void superfault( int n );	/* See boot.s */

/*
 * Vector to C language ISR.
 */

void irq_dispatch( void )
{
	(* (void (*)(void)) AIC->ivr)();
}

/*
 * Try to recover from nonsense.
 */
 
void irq_fault( void )
{
	superfault( 6 );	/* See boot.s */
}

/*
 * Fill the vectors with stuff.
 */

void irq_dispatch_init( void )
{
	int i;
	for( i = 0; i < 32; i += 1 ) AIC->svr[ i ] = (uint32_t) irq_fault;
	AIC->spu = (uint32_t) irq_fault;
}

/*
 * $Log$
 * Revision 1.1  2010-06-07 18:33:44  jpd
 * AIC driver.
 *
 */

