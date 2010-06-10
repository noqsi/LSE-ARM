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
	AIC->eoicr = 0;
}

/*
 * Try to recover from nonsense.
 */
 
void irq_fault( void )
{
	AIC->eoicr = 0;		/* acknowledge the interrupt to AIC */
	superfault( 6 );	/* See boot.s */
}

/*
 * Get back to prompt. Does this really belong here?
 */
 
void user_interrupt( void )
{	
	AIC->eoicr = 0;		/* acknowledge the interrupt to AIC */	
	superfault( 8 );	/* See boot.s */
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
 * Revision 1.3  2010-06-10 17:53:07  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
 *
 * Revision 1.2  2010-06-08 18:57:41  jpd
 * Faults and user interrupts now work on SAM7A3
 *
 * Revision 1.1  2010-06-07 18:33:44  jpd
 * AIC driver.
 *
 */

