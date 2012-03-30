/* SPI driver for an Analog Devices "pulsar" ADC. */

/*

After much experimentation, I find that the toxic feature of the SPI
appears to be the CSAAT bit. When I don't use it, I get comprehensible
behavior, but when I use it everything rapidly becomes AFU. Unfortunately,
the Pulsar needs the CS bit low until a new conversion is wanted, at least
in three wire mode. So, here I bitbang the CS. That causes mode faults, but
MODFDIS appears to work in this case.

*/
#include "memory_7a3.h"
#include "pulsar_spi.h"
#include "spi.h"
#include "pio.h"

/* 

This version can only read 16 bits because to read more, two SPI transfers
are required, and for less we'd have to fiddle with sign extension.
Should work OK for ADC's with different bit numbers:
it just drops extra bits or appends junk LSB's.
It also assumes only one ADC on the SPI port, controlled by NPCS0,
and nothing else on that port. It assumes PIO bits set up properly elsewhere.
Use CSAAT mode to keep the ADC selected except when we want a conversion,
because that's how the ADC wants to be used.

*/

#define CS0 0x800	/* Chip select bit */
#define CONV 4		/* how long to spin to make CONV */

void pulsar_spi_init( struct spi *spi )
{
	/*
	The SPI interface is touchy about 
	programming order and configuration.
	To avoid the dreaded mode fault, configure 
	pins before calling this function. Then,
	this initialization order seems to work OK.
	*/
	
	PIOA->codr = CS0;	/* prepare for conversion */
	spi->cr = SWRST;	/* software reset of SPI */
	spi->mr = MSTR | MODFDIS;		/* master mode */
	spi->csr[0] = MAKE_SCBR(4) | MAKE_BITS(16);
	spi->cr = SPIEN;		/* enable */
}
	
	

int16_t pulsar_spi_read( struct spi *spi )
{
	int i;
	
	/* initiate conversion */

	for( i = 0; i < CONV; i += 1) {
		PIOA->sodr = CS0;
		PIOA->sodr = CS0;
		PIOA->sodr = CS0;
	}
	
	PIOA->codr = CS0;		/* enable readout */	
	
	spi->tdr = 0xf00d;		/* read bits */
	
	while( !(spi->sr & RDRF)) ;	/* spin, wait for transfer */
	
	return (int16_t) spi->rdr;	/* result */
}

