/* $Id$ */

/* Bit banging driver for a 16 bit Analog Devices "pulsar" ADC. */

#include "lse.h"
#include "pulsar.h"

static inline int_16 grabbit( void )
{
	PIO->codr = SCK;		/* Clock bit out */
	PIO->sodr = SCK;		/* Leave SCK high */
	return (PIO->pdsr & SDO) != 0;	/* Grab the bit */
}

void pulsar16( void )
{
	int_16 val = 0;
	int i;
	
	PIO->sodr = CNV;		/* Pulse CNV faster than conversion time */
	PIO->codr = CNV;
	
	while( PIO->pdsr & SDO );	/* Wait for SDO low */
	
	for( i = 0; i < 16; i += 1 ) {	/* Grab 16 bits */
		val <<= 1;
		val |= grabbit();
	}
	
	(void) grabbit();		/* Release SDO */

	*--sp = val;
}

