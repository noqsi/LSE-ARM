/* $Id$ */

#include "spi_driver.h"
#include "lse-arm.h"

static void spi_addr( void ) { *sp = (cell) spi_devices[*sp].spi; }

// inq outq unit spi-initq yields nothing

static void initq( void )
{
	int unit = *sp++;
	uint32_t *outq = (uint32_t *) *sp++;
	uint32_t *inq = (uint32_t *) *sp++;
	spi_initq( unit, inq, outq );
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
		left -= read;
		buf += read;
	}
}




void spi_primitives( void )
{
	build_primitive( spi_addr, "&spi" );
	build_primitive( initq, "spi-initq");
	build_primitive( read, "spi-read" );
	build_primitive( write, "spi-write" );
}

/*
 $Log$
 Revision 1.3  2010-09-14 00:01:36  jpd
 SPI working in local loopback mode.

 Revision 1.2  2010-09-06 20:28:20  jpd
 *** empty log message ***

 Revision 1.1  2010-07-16 20:10:40  jpd
 More additions to SPI driver.
 Cleanup from CVS screwup.

 */
