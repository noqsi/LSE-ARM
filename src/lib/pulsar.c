/* $Id$ */

/* Bit banging driver for an Analog Devices "pulsar" ADC. */

#include "pio.h"
#include "pulsar.h"

#define MAX_WAIT 1000	/* polling longer than this indicates trouble */

/* 
 * Do the bit-banging.
 * This should be called with interrupts masked. 
 */

int32_t pulsar_read( struct pulsar *p )
{
	uint32_t val;
	unsigned i;
	
	p->pio->sodr = p->cnv;		/* Pulse CNV faster than conversion time */
	p->pio->codr = p->cnv;
	p->pio->sodr = p->sck;		/* make sure sck is high */
	
	i = 0;
	val = 0;
	
	while( p->pio->pdsr & p->sdo ) 	/* Wait for SDO low */
		if( ++i > MAX_WAIT ) return PULSAR_TIMEOUT;
	
	for( i = 0; i < p->bits; i += 1 ) {	/* Grab the bits */
		val <<= 1;
		p->pio->codr = p->sck;
		p->pio->sodr = p->sck;
		if( p->pio->pdsr & p->sdo ) val |= 1;
	}
	
	p->pio->codr = p->sck;		/* Release SDO */

	if( val & p->snx ) val |= p->snx;
	
	return (int32_t) val;
}

