/* $Id$ */

/* SPI driver for an Analog Devices "pulsar" ADC. */

#include "pulsar_spi.h"

/* 

This version can only read 8-16 bits because to read more, two SPI transfers
are required. Should work OK for an 18 bit ADC, it just drops the last
two bits. It also assumes only one ADC on the SPI port, and nothing else on that port. It assumes PIO bits set up properly elsewhere.

*/

void pulsar_spi_init( struct pulsar_spi *p )
{
	p->spi->cr = SWRST;	/* software reset of SPI */
	p->spi->mr = MSTR | PS;		/* master mode */
	p->spi->csr[0] = MAKE_SCBR(4) | MAKE_BITS(p->spi->bits) | CSAAT;
	p->spi->csr[1] = MAKE_SCBR(4) | MAKE_BITS(p->spi->tconv);
	p->spi->cr = SPIEN;		/* enable */
}
	
	

int16_t pulsar_spi_read( struct pulsar_spi *p )
{
	uint16_t val;

	p->spi->tdr = MAKE_PCS(1);	/* Force deselect of channel zero */
	
	while( !(p->spi->sr & RDRF)) ;	/* spin, wait for conversion */
	
	(void) p->spi->rdr;		/* dump useless result */
	
	p->spi->tdr = 0;		/* read bits */
	
	while( !(p->spi->sr & RDRF)) ;	/* spin, wait for transfer */
	
	val = p->spi->rdr;		/* result */
	
	if( val & p->snx ) val |= p->snx;	/* sign extend as needed */
	
	return (int16_t) val;
}

