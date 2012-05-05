/* $Id$ */

/* sequencer.c is only used by TESS, which uses the SAM7x256 board.
   So we know which memory file to include? ~Matt */
#include "memory_7x256.h"

/*************************/

#include "pio.h"
#include "pmc.h"
#include "peripheral_id_7x256.h"
#include "sequencer.h"
#include "sequencer_bits.h"

void sequencer_port_setup( void )
{
	PMC->pcer = bit(PIOB_ID);	/* enable PIOB clock */
	PIOB->codr = SEQ_INPUTS;	/* We'll drive 'em to zero to start */
	PIOB->oer = SEQ_INPUTS;		/* seq inputs are proc outputs */
	PIOB->odr = SEQ_OUTPUTS | SEQ_DATA;
	PIOB->per = SEQ_BITS;		/* enable pio on the pins */
	PIOB->ower = SEQ_INPUTS | SEQ_DATA;	/* enable simple output */
}

/*
 * Strobe will be asserted as part of data read/write.
 * The following function waits for ACK and deasserts it.
 */

static void inline ackwait( void )
{
	while( !(PIOB->pdsr & SEQ_ACK));	/* wait for ack */
	PIOB->codr = SEQ_STR;			/* deassert strobe */
}

/*
 * Before we fiddle with the port data, wait for ACK deasserted.
 */

static void inline busywait( void )
{
	while((PIOB->pdsr & SEQ_ACK));	/* wait for ack deasserted */
}

/*
 * Wait for both ACK and CMD_FIFO_FULL deasserted 
 */

static void inline fifowait( void )
{
	while((PIOB->pdsr & (SEQ_ACK | SEQ_CMD_FIFO_FULL)));
}



void sequencer_write( int addr, int data )
{
	PIOB->oer = SEQ_DATA;		/* Allow data out */
	busywait();
	PIOB->odsr = data | (addr<<SEQ_ADDR_SHIFT) | SEQ_STR;
	ackwait();
}

int sequencer_read( int addr )
{
	PIOB->odr = SEQ_DATA;		/* disable data outputs */
	busywait();	
	PIOB->odsr = (addr<<SEQ_ADDR_SHIFT) | SEQ_RNW | SEQ_STR;
	ackwait();
	return PIOB->pdsr & SEQ_DATA;
}

#define min(x,y)	(((x)<(y))?(x):(y))

void sequencer_go( int start_block, int blocks, int count )
{
	PIOB->oer = SEQ_DATA;		/* Allow data out */
		
	if( 1 == blocks ) {	/* Use the hardware repeat */
		while( count > 0 ) {
			int c = min( 32, count );
			fifowait();
			PIOB->odsr = (start_block<<12) | (c-1) |
				(SEQ_CMD_DAT<<SEQ_ADDR_SHIFT) | SEQ_STR;
			count -= c;
			ackwait();
		}
	}
	else
	{			
		while( count-- ) {
			int b = start_block;
			int n = blocks;
			while( n-- ) {
				fifowait();
				PIOB->odsr = (b++<<12) |
				(SEQ_CMD_DAT<<SEQ_ADDR_SHIFT) | SEQ_STR;
				ackwait();
			}
		}
	}
}
	

/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.3  2010-06-04 18:13:17  jpd
 * Update build for multiple targets.
 *
 * Revision 1.2  2009-03-26 02:10:13  jpd
 * Can now compile seq stuff.
 *
 * Revision 1.1  2009-03-26 01:45:01  jpd
 * First draft of sequencer driver.
 *
 */
 
