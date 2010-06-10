/* $Id$ */

#include "pit_driver.h"

/* exported variables */

uint64_t volatile ticks = 0;

void (* volatile on_tick)( void ) = 0;		/* callout */


static struct pit *p;			/* need to know where it is on this processor */

extern unsigned const mck_hz;		/* and what MCK is */

void pit_isr( void )
{
	ticks += PICNT( p->pivr );
	if( on_tick ) (* on_tick)();	/* do the callout */
}

void init_pit( struct pit *pit, unsigned hz )
{
	unsigned pclk = mck_hz/16;
	
	p = pit;
	p->mr = PITEN | PITIEN | (((pclk + hz/2)/hz)-1);
}


/*
 * $Log$
 * Revision 1.1  2010-06-10 17:53:07  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
 *
 */
