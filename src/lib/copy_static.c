/* $Id$ */

/* Copy the initial values of
 * intialized static variables to working memory.
 */
 
#include <string.h>

extern int data_load_[], data_start_[], data_end_[];

void copy_static( void )
{
	bcopy( data_load_, data_start_, 
		(void *) data_end_ - (void *) data_start_ );

}

/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 */
