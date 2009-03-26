/* $Id$ */

#include "sequencer.h"
#include "lse.h"

/*
 * addr SeqRead yields data
 */

void readSeq( void )
{
	*--sp = (cell) sequencer_read( (int) *sp++ );
}

/*
 * addr data SeqWrite yields nothing
 */

void writeSeq( void )
{
	int data = (int) *sp++;		/* force evaluation order */
	sequencer_write( (int) *sp++, data );
}

/*
 * count seg goSeq yields nothing
 *
 * seg is the address of a starting block id and a count of blocks in the
 * segment.
 *
 */

void goSeq( void )
{
	cell *seg = (cell *) *sp++;		/* force evaluation order */
	sequencer_go( (int) seg[0], (int) seg[1], (int) *sp++ );
}

void SeqPrimitives( void )		/* put 'em in the LSE dictionary */
{
	build_primitive( readSeq, "readSeq" );
	build_primitive( writeSeq, "writeSeq" );
	build_primitive( goSeq, "goSeq" );
}

/*
 * $Log$
 * Revision 1.1  2009-03-26 01:45:01  jpd
 * First draft of sequencer driver.
 *
 */
