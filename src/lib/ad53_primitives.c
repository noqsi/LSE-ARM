#include "ad53bb.h"
#include "lse-arm.h"

/* List of AD53 DACs provided by application. */

extern struct ad53 ad53_dac[];

static void write_ad53( void )
{
	unsigned id = *sp++;
	
	ad53_write( ad53_dac + id, (uint16_t) *sp++ );
}

void ad53_primitives( void )
{
	build_primitive( write_ad53, "!ad53" );
}

