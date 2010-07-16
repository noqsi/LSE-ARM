/* $Id$ */

#include "spi.h"

#define EMPTY 0xffffffff

struct spi_q {
	uint32_t * volatile ip, 	/* input pointer */
		* volatile op,		/* output pointer */
		*q,		/* storage */
		len;		/* # of words in storage */
};

struct spi_parameters {
	struct spi *spi;	/* pointer to the hardware device */
	struct spi_q inq, outq;		/* the queues */
};

extern struct spi_parameters *spi_devices;

extern void spi_initq( int un, uint32_t *inbuf, uint32_t *outbuf );
extern void spi_init( struct spi_parameters *p, int n );
extern int spi_read( int un, uint32_t *buf, int n );
extern int spi_write( int un, uint32_t *buf, int n );
extern void spi_isr( int un );

/*
 * $Log$
 * Revision 1.2  2010-07-16 20:10:39  jpd
 * More additions to SPI driver.
 * Cleanup from CVS screwup.
 *
 * Revision 1.1  2010-07-13 18:38:07  jpd
 * First draft of low level SPI driver.
 *
 */
