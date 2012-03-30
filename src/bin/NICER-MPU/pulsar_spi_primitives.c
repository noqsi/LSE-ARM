#include "pulsar_spi.h"
#include "lse-arm.h"

/* Application must provide a pointer to the SPI */

extern struct spi *pulsar_spi;

static void read_pulsar( void )
{
	*--sp = (cell) pulsar_spi_read( pulsar_spi );
}

void pulsar_spi_primitives( void )
{
	build_primitive( read_pulsar, "@pulsar" );
}

