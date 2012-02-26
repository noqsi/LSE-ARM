/*
 * bit-bang write of AD5308 et al. DACs
 */

#include "pio.h"
#include "ad53bb.h"

/* 
 * This function just writes 16 bits. 
 * The caller must assemble the 16 bit command. 
 */


void ad53_write( struct ad53 *p, uint16_t v )
{
	int i;
	
	p->pio->sodr = p->sync;		/* Be sure SYNC is high to start */
	p->pio->codr = p->sclk;		/* and SCLK low */
	
	p->pio->codr = p->sync;		/* thus we begin */
	
	for( i = 0; i < 16; i += 1 ) {
		if( v & 0x8000 ) p->pio->sodr = p->din;
		else p->pio->codr = p->din;
		p->pio->sodr = p->sclk;
		p->pio->codr = p->sclk;
		v <<= 1;
	}
	
	p->pio->sodr = p->sync;		/* That's all, folks */

}
		
