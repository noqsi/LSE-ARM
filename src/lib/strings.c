/* $Id$ */

/*
    Copyright 2004, 2005, 2006 John P. Doty and Matthew P. Wampler-Doty

    This file is part of LSE64.

    LSE64 is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    LSE64 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LSE64; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "lse-arm.h"
#include <sys/param.h>
#include <stdlib.h>


/*
 * cstring() pops an LSE string pointer from the stack, returning
 * a pointer to an equivalent C char string.
 */

char *cstring( char *b, int n )
{
	cell *s = (cell *)(intptr_t)*sp++;
	int nc, nseg, i;
	wchar_t wb[MAXPATHLEN+1], *wbp;
	static char sb[MAXPATHLEN+1];
	char *bp;
	
	if( !b ) { b = sb; n = MAXPATHLEN+1; }  /* use the static buffer */
	b[--n] = 0;								/* force null termination */
	bp = b;
	
	for( nc = *s++; nc > 0; nc -= MAXPATHLEN ) {
		if( nc > MAXPATHLEN ) nseg = MAXPATHLEN; else nseg = nc;
		wbp = wb;
		for( i = 0; i < nseg; i += 1 ) *wbp++ = *s++;
		*wbp++ = 0;
		n -= wcstombs( bp, wb, n );
		if( n <= 0 ) break;
	}
	return b;
}

/*
 * ToNumber converts a string to a number.
 * If the whole string can be converted assuming it's an integer,
 * an integer results and the flag is 1.
 * Otherwise, if the whgole string can be converted assuming it's
 * a floating point number, a floating point number results and the flag is 1.
 * Otherwise, 0 results and the flag is 0,
 */

void ToNumber( void )
{
	char *b = cstring( 0, 0 );
	char *ep;
	cell i = strtoll( b, &ep, 10 );
	double d;
	
	if( !*ep ) {		/* found the null termination */
		*--sp = i;
		flag = 1;
		return;
	}
	
	d = strtod( b, &ep );
	
	if( !*ep ) {		/* found the null termination */
		*(fcell *)--sp = d;
		flag = 1;
		return;
	}
	
	*--sp = flag = 0;   /* conversion failed */
}

/*
 * HexNumber converts a string to a number.
 * If the whole string can be converted,
 * an integer results and the flag is 1.
 * Otherwise, 0 results and the flag is 0,
 */

void HexNumber( void )
{
	char *b = cstring( 0, 0 );
	char *ep;
	cell i = strtoull( b, &ep, 16 );
	
	if( !*ep ) {		/* found the null termination */
		*--sp = i;
		flag = 1;
		return;
	}
	
	*--sp = flag = 0;   /* conversion failed */
}
	

/*
 * $Log$
 * Revision 1.3  2009-03-11 02:19:42  jpd
 * It compiles, executes.
 * Prompt doesn't work.
 * OS hooks need removal.
 *
 * Revision 1.2  2009-03-10 20:37:11  jpd
 * Makefile, ports.
 *
 * Revision 1.1.1.1  2009-03-10 19:08:44  jpd
 * Initial sources
 *
 */


