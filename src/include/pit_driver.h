/* $Id$ */

#include "pit.h"


/* Interface Variables */

extern uint64_t volatile ticks;

extern void (* volatile on_tick)( void );		/* callout */

/* Functions */

extern void pit_isr( void );
extern void init_pit( struct pit *pit, unsigned hz );

/*
 * $Log$
 * Revision 1.1  2010-06-10 17:53:07  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
 *
 */
