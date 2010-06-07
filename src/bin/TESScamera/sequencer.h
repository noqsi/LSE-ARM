/* $Id$ */

/* Sequencer driver prototypes */

extern void sequencer_port_setup( void );
extern void sequencer_write( int addr, int data );
extern int sequencer_read( int addr );
extern void sequencer_go( int start_block, int blocks, int count );

/*
 *$Log$
 *Revision 1.1  2010-06-07 00:39:01  jpd
 *Massive reorganization of source tree.
 *
 *Revision 1.1  2009-03-26 01:45:01  jpd
 *First draft of sequencer driver.
 *
 */
