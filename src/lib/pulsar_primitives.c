#include "pulsar.h"
#include "lse-arm.h"

/* List of Pulsar ADC's provided by application. */

extern struct pulsar pulsar_adc[];

static void read_pulsar( void )
{
	*sp = (cell) pulsar_read( pulsar_adc + *sp );
}

void pulsar_primitives( void )
{
	build_primitive( read_pulsar, "@pulsar" );
}

