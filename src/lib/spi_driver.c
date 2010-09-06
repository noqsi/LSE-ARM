/* $Id$ */

#include "spi_driver.h"

/* Keep track of the devices */

struct spi_parameters *spi_devices;

/*
Initialize the devices specified by the array p.
The number to intialize is given by n.
The "spi" members of the array should be intialized by the caller,
and the array should be static.
*/

void spi_init( struct spi_parameters *p, int n )
{
	int i;
	
	spi_devices = p;
	
	for( i = 0; i < n ; i += 1 )
		p[i].spi->cr = SWRST;
}

/*
Initialize the queues for a given unit un.
The arrays are LSE-style buffers, whose first element
is the count of elements in the remainder.
*/

void spi_initq( int un, uint32_t *inbuf, uint32_t *outbuf )
{
	struct spi_parameters *p = spi_devices + un;
	
	p->inq.len = *inbuf;
	p->inq.ip = p->inq.op = p->inq.q = inbuf + 1;
	
	p->outq.len = *outbuf;
	p->outq.ip = p->outq.op = p->outq.q = outbuf + 1;
	

}	

static int putq( struct spi_q *qp, uint32_t x )
{
	uint32_t *next = qp->ip + 1;

	if( next >= qp->q + qp->len ) next = qp->q;
	if( next == qp->op ) return 0;		/* full */
	*(qp->ip) = x;
	qp->ip = next;
	return 1;
}

static uint32_t getq( struct spi_q *qp )
{
	uint32_t *this, *next;
	
	if( qp->ip == qp->op ) return EMPTY;
	this = qp->op;
	next = qp->op + 1;
	if( next >= qp->q + qp->len ) next = qp->q;
	qp->op = next;
	return *this;
}	

int spi_read( int un, uint32_t *buf, int n )
{
	struct spi_parameters *p = spi_devices + un;
	int r;			/* number read */
	uint32_t w;		/* word from q */
	struct spi_q *q = &(p->inq);
	
	p->spi->ier = RDRF;	/* turn on the intr */
	
	for( r = 0; r < n; r += 1 ) {
		w = getq( q );
		if( w == EMPTY ) break;
		*buf++ = w;
	}
	
	return r;
}
		
int spi_write( int un, uint32_t *buf, int n )
{
	struct spi_parameters *p = spi_devices + un;
	struct spi_q *q = &(p->inq);
	int r;			/* number written */

	for( r = 0; r < n; r += 1 ) {
		int c = putq( q, *buf++ );
		if( c == 0 ) break;		/* q is full */
	}
	
	p->spi->ier = TDRE;	/* turn on the intr */
	
	return r;	/* actual number written */
}

	
void spi_isr( int un )
{
	struct spi_parameters *p = spi_devices + un;
	unsigned sr = p->spi->sr;

	
	if( sr & RDRF ) (void) putq( &(p->inq), p->spi->rdr );	/* ignore overrun */
	if( sr & TDRE ) {
		uint32_t t = getq( &(p->outq) );
		if( t == EMPTY ) 
			p->spi->idr = TDRE;	/* nothing to send: disable intr */
		else
			p->spi->tdr = t;	/* send it */
	}
}	

/*
 * $Log$
 * Revision 1.3  2010-09-06 20:28:20  jpd
 * *** empty log message ***
 *
 * Revision 1.2  2010-07-16 20:10:40  jpd
 * More additions to SPI driver.
 * Cleanup from CVS screwup.
 *
 * Revision 1.1  2010-07-13 18:38:07  jpd
 * First draft of low level SPI driver.
 *
 */
