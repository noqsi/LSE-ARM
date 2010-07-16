/* $Id$ */

#include "spi_driver.h"
#include "lse-arm.h"

static void spi_addr( void ) { *sp = (cell) spi_parameters[*sp].spi; }

// inq outq unit spi-initq yields nothing

static void initq( void )
{
	int unit = *sp++;
	uint32_t outq = *sp++;
	uint32_t inq = *sp++;
	spi_initq( int unit, inq, outq );
}

// buf unit spi-write yields nothing

static void write( void )
{
	int unit = *sp++;
	uint32_t *buf = (uint32_t *) *sp++;
	int left = *buf++;
	
	while( left > 0 )
	{
		int written = spi_write( unit, buf, left );
		left -= written;
		buf += written;
	}
}

// buf unit spi-read yields nothing

static void read( void )
{
	int unit = *sp++;
	uint32_t *buf = (uint32_t *) *sp++;
	int left = *buf++;
	
	while( left > 0 )
	{
		int read = spi_read( unit, buf, left );
		left -= written;
		buf += written;
	}
}




void spi_primitives( void )
{
	build_primitive( spi_addr, "&spi" );
	build_primitive( initq, "spi-initq");
	build_primitive( read, "spi-read" );
	build_primitive( write, "spi_write" );
}

/*
 $Log$
 Revision 1.1  2010-07-16 20:10:40  jpd
 More additions to SPI driver.
 Cleanup from CVS screwup.

 */
