/* $Id$ */

/*
    Copyright 2004, 2005, 2006, 2009 John P. Doty and Matthew P. Wampler-Doty

    This file is part of LSE-ARM.

    LSE-ARM is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    LSE-ARM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LSE-ARM; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "lse-arm.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>

/*
 * Basic memory access
 */
 
void fetch( void ) { *sp = *(cell*)(intptr_t)*sp; }
void fetchp( void ) { *sp = (intptr_t)*(cell**)(intptr_t)*sp; }
void store( void ) { cell *w = (cell *)(intptr_t)*sp++; *w = *sp++; }
void storep( void ) { cell **w = (cell **)(intptr_t)*sp++; *w = (cell *)(intptr_t)(*sp++); }
void append( void ) {		/* @!+ */
	cell *vpp = (cell *)(intptr_t)*sp++; 
	cell *vp = (cell *)(intptr_t) *vpp; 
	*vp++ = *sp++; *vpp = (intptr_t) vp; }
void scan( void ) {		/* @@+ */
	cell *vpp = (cell *)(intptr_t)*sp++;
	cell *vp = (cell *)(intptr_t) *vpp; 
	*--sp = *vp++; *vpp = (intptr_t) vp; }
/*
 * Constants and variables
 */

void stringconstant( void ) { cell *p = (cell *)(intptr_t)*lc; *--sp = (intptr_t) (p + D_DATA ); }
void constant( void ) { cell *p = (cell *)(intptr_t)*lc; *--sp = p[ D_DATA ]; }
void variable( void ) { cell *p = (cell *)(intptr_t)*lc; *--sp = (intptr_t)(p + D_BODY); }

/*
 * Arithmetic
 */
 
void add( void ) { cell w = *sp++; *sp += w; }
void subtract( void ) { cell w = *sp++; *sp -= w; }
void multiply( void ) { cell w = *sp++; *sp *= w; }
void divide( void ) { cell w = *sp++; *sp /= w; }
void mod( void ) { cell w = *sp++; *sp %= w; }
void neg( void ) { *sp = -*sp; }

void fadd( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp += w; }
void fsubtract( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp -= w; }
void fmultiply( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp *= w; }
void fdivide( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp /= w; }
void fmodd( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp = fmod(*(fcell*)sp, w); }
void fneg( void ) { *(fcell*)sp = -*(fcell*)sp; }

void and( void ) { cell w = *sp++; *sp &= w; }
void or( void ) { cell w = *sp++; *sp |= w; }
void xor( void ) { cell w = *sp++; *sp ^= w; }
void not( void ) { *sp = ~*sp; }

void ffloat( void ) { * (fcell *)sp = (fcell) *sp; }
void fix( void ) { *sp = (cell) *(fcell *)sp; }

/*
 * Stack Gymnastics
 */
 
void dupp( void ) { cell w = *sp; *--sp = w; }
void swap( void ) { cell w = *sp; *sp = sp[1]; sp[1] = w; }
void drop( void ) { sp += 1; }
void pick( void ) { *sp = sp[ *sp ]; }
void rot( void ) { cell w0 = sp[0], w1 = sp[1]; sp[0] = sp[2]; sp[1] = w0; sp[2] = w1; }

/*
 * Predicates
 */

void equal( void ) { cell w = *sp++; flag = ( *sp++ == w ); }
void greater( void ) { cell w = *sp++; flag = ( *sp++ > w ); }
void less( void ) { cell w = *sp++; flag = ( *sp++ < w ); }
void equal0( void ) { flag = ( *sp++ == 0 ); }
void greater0( void ) { flag = ( *sp++ > 0 ); }
void less0( void ) { flag = ( *sp++ < 0 ); }

void fequal( void ) { fcell w = *(fcell*)sp++; flag = ( *(fcell*)sp++ == w ); }
void fgreater( void ) { fcell w = *(fcell*)sp++; flag = ( *(fcell*)sp++ > w ); }
void fless( void ) { fcell w = *(fcell*)sp++; flag = ( *(fcell*)sp++ < w ); }
void fequal0( void ) { flag = ( *(fcell*)sp++ == 0. ); }
void fgreater0( void ) { flag = ( *(fcell*)sp++ > 0. ); }
void fless0( void ) { flag = ( *(fcell*)sp++ < 0. ); }

void setflag( void ) { flag = 1; }
void clearflag( void ) {flag = 0; }

/*
 * Conditional and unconditional return
 */

void exitrue( void ) { if( flag ) lc = (cell *)(intptr_t)*rsp++; }
void exitfalse( void ) { if( !flag ) lc = (cell *)(intptr_t)*rsp++; }
void exitword( void ) { lc = (cell *)(intptr_t)*rsp++; }

/*
 * Tail call
 */

void jfalse( void ) { 		/* jump if false */
	cell t = *++lc; 
	if( !flag ) lc = ((cell *)(intptr_t) t) + D_BODY - 1 ;
}

/*
 * Goeke-style unstructured flow control.
 */
 
void iftrue( void ) { if( !flag ) lc+=1; }
void iffalse( void ) { if(flag) lc+=1;  }
void repeat( void ) { lc = ((cell *)(intptr_t)(*rsp++)) - 1; }
void repeatrue( void ) { if( flag ) lc = ((cell *)(intptr_t)(*rsp++)) - 1; }
void iterate_init( void ) { cell c = *sp++; if( c <= 0 ) lc+=2; else *--rsp = c; }
void iterate( void ) { if( --(*rsp) > 0 ) lc -= 2; else rsp += 1; }
void anditerate( void ) { if( (--(*rsp) > 0)  && flag ) lc -= 2; else rsp += 1; }
void count( void ) { *--sp = rsp[1]; }

/*
 * Return stack
 */
 
void s2r( void ) { *--rsp = *sp++; }
void r2s( void ) { *--sp = *rsp++; }
void rcopy( void ) { *--sp = *rsp; }
void rdrop( void ) { rsp += 1; }

/*
 * Misc
 */

void literal( void ) { *--sp = *++lc; } /* quote: push next cell onto stack */
void noop( void ) {}					/* do nothing */
void cells( void ) { cell w = *sp++; *sp += w * sizeof( cell ); }   /* offset by cells */

/*
 * Character I/O
 */  
 
extern char init_lse[], app_lse[], readchar( void );
extern void writechar( char c );
  

void put( void )
{
    writechar( *sp++ );
	flag = 1;
}

static int have_ungotten;
static char ungotten;

void unget( void )
{
	if( have_ungotten ) {
		flag = 0;
		drop();
		return;
	}
	have_ungotten = 1;
	ungotten = *sp++;
	flag = 1;
}

#define INBUFSZ 256

cell FlowPrompt = -1;			/* if >= 0, prompt lines with this */

/*
Call primitive_io_abort after a fault to clear pending input.
*/

static int abort_input;

void primitive_io_abort( void )
{
	abort_input = 1;
}

// int char_ready( void )
// {
//	return 1; /* stub */
// }

void get_nowait( void )
{
	extern char * const init_source[];
	static char * const *source_block = init_source;
	static char *source_char;
	static char inbuf[INBUFSZ];
	static char *ibp;
	static char *const inbuf_source[] = { inbuf, 0 };
	
	flag = 1;	/* assume success */
	
	if( abort_input ) {
		have_ungotten = 0;
		source_block = inbuf_source + 1;	/* null */
		ibp = 0;
		abort_input = 0;
	}
	
	if( have_ungotten ) {
		*--sp = ungotten;
		have_ungotten = 0;
		return;
	}
	
	while( *source_block ) {		/* get input from memory */
	
		if( !source_char ) {		/* point to start of block */
			source_char = *source_block;
		}
		
		if( *source_char ) {		/* hey, we got one! */
			*--sp = *source_char++;
			return;
		}
		
		/* 
		at this point, we have a null character, so
		attempt to move to next block.
		*/
		
		source_block += 1;
		source_char = 0;
	}
	
		
	/* If we get here, there is no more input in memory,
	so grab a line from the input stream */
				
	if( ibp == 0 ) {	/* start new line */
		ibp = inbuf;
		if( FlowPrompt >= 0 ) writechar( FlowPrompt );
	}
	
	if( char_ready() ) {
		char c = readchar();
			
		if( c == '\n' ) {	/* end of line */
			*ibp++ = c;
			*ibp++ = 0;
			ibp = 0;
			source_block = inbuf_source;
			get_nowait();	/* tail recursion */
			return;
		}
		
		/* if we reach here, we won't have a char to return */
		
		flag = 0;
			
		if( c == '\b' ) {
			writechar( ' ' );
			if( ibp > inbuf ) {	/* backspace, erase last */
				writechar( '\b' );
				ibp -= 1;
			}
			return;
		}
			
		if( ibp >= inbuf + INBUFSZ - 3 ) {	/* buffer full */
			writechar( 7 );			/* bell */
			return;
		}
			
		*ibp++ = c;
	}
	
	flag = 0;	/* fail */
}

void get( void )
{
	do { get_nowait(); } while( !flag );
}

/*
 * skip_space skips over whitespace in input.
 * If it sets the flag to true, the next get() will obtain a non-space character.
 * Otherwise, a false flag indicates that it encountered end of line, end of file, or an I/O error.
 */

void skip_space( void )
{
	for(;;){
		get();
		if( !flag ) { drop(); return; }
		if( !isspace( *sp )) {
			unget();
			flag = 1;
			return;
		}
		if( *sp++ == '\n' ) {
			flag = 0;
			return;
		}
	}
}

/*
 * Skip characters up to and including the next newline.
 * Yields false on EOF or error, otherwise true.
 */

void skip_to_nl( void )
{
	for(get(); flag && *sp != '\n'; get()) drop();
	drop();
}

void get_token( void )
{
	int n;
	
	skip_space();
	if( !flag ) return;
	
	for( n = 0;; n += 1) {
		get();
		if( !flag ) break;
		if( isspace( *sp ) ) break;
		constop[ D_DATA + 1 + n ] = *sp++;
	}
	constop[ D_DATA ] = n;
	if( flag ) unget(); else drop();
	flag = 1;
}
    
static void print_string( cell * s )	/* handy for debugging */
{
	int len = *s++;
	
	while( len-- ) { *--sp = *s++; put(); }
}

void put_string( void ) { print_string( (cell *)(intptr_t)*sp++ ); }

/*
 * fixate_literal_string() takes a temporary string at the end of the
 * string dictionary. It checks to see if the string is already present
 * in the dictionary. If not, it makes it into a new definition. In either
 * case, it pushes the permanent address of the string on the stack.
 */

void fixate_literal_string( void )
{
	cell *tmp = constop + D_DATA;
	cell *ent;
	for( ent = constlast; ent ; ent = (cell *) (intptr_t)ent[ D_PREV ] ) {	/* look for previous definition */
		cell *t = tmp;
		cell *e = ent + D_DATA;
		cell n = *t++;
		if( ent[ D_PRIM ] != (cell) stringconstant ) continue;  /* not a string */
		if( n != *e++ ) continue;					/* lengths don't match */
		while( n-- ) if( *t++ != *e++ ) break;   /* strings don't match */
		if( n < 0 ) {							/* match! */
			*--sp = (intptr_t)(ent + D_DATA);		/* we have a match, return it */
			return;
		}
	}
	/* If we get to here, nothing matched. Make new entry of the temp.
	 * Note that the string itself is already in the right place:
	 * we just have to build a literal dictionary entry around it,
	 * and adjust constop and constlast.
	 */
	constop[ D_PRIM ] = (cell) stringconstant;
	constop[ D_PREV ] = (cell)(intptr_t) constlast;
	constop[ D_PRECEDENCE ] = DP_COMPILED;
	constlast = constop;
	constop += D_DATA + 1 + *tmp;
	*--sp = (intptr_t) (tmp);
}

/* get the next word name in the input stream */

void get_name( void )
{
	get_token();
	if( flag ) fixate_literal_string();
	else *--sp = 0;		/* placeholder for string pointer on failure */
}

void make_def( void )		/* expects a pointer to the name literal on the stack */
{
	deftop[ D_PRIM ] = (cell) noop;			/* mostly harmless default */

	deftop[ D_PRECEDENCE ] = DP_COMPILED;   /* default */
	deftop[ D_NAME ] = (intptr_t) *sp++;				/* grab name from stack */
	deftop[ D_PREV ] = (intptr_t) deflast;
	deflast = deftop;						/* now we're it */
	deftop = deftop + D_BODY;				/* initially, body is empty */
}

void interpret( void ) { 
	*--rsp = (intptr_t) lc; lc = ((cell *) (intptr_t) *lc) + D_BODY - 1; 
}

void fixate_colon_def( void )
{
	deflast[ D_PRIM ] = (cell) interpret;
	deftop = (cell *) (intptr_t) cptr;
}

void named_constant( void ) { *--sp = ((cell *)(intptr_t)*lc)[D_BODY]; }

void make_named_constant( void )	/* value, pointer to name literal on stack */
{
	make_def();
	deflast[ D_PRIM ] = (cell) named_constant;
	*deftop++ = *sp++;
}

void make_array( void )  /* size, pointer to name literal on stack */
{
	cell n;
	make_def();
	deflast[ D_PRIM ] = (cell) variable;
	for( n = *sp++; n > 0; n -= 1 ) *deftop++ = 0;
}

void make_constant( void )  /* expects the constant value on the stack */
{
	cell *ent;
	
	for( ent = constlast; ent ; ent = (cell *)(intptr_t) ent[ D_PREV ] ) {	/* look for previous definition */
		if( ent[D_PRIM] == (cell) constant && ent[D_DATA] == *sp ) {	/* got a match */
			*sp = (intptr_t) ent;											/* return it */
			return;
		}
	}
	constop[ D_PRIM ] = (intptr_t) constant;	/* no match, build new entry */
	constop[ D_PREV ] = (intptr_t) constlast;
	constop[ D_PRECEDENCE ] = DP_COMPILED;
	constop[ D_DATA ] = *sp;
	constlast = constop;
	constop += D_DATA + 1;
	*sp = (intptr_t) constlast;					/* return it */
}


void find( void )
{
	cell *def;
	for( def = deflast; def; def = (cell *)(intptr_t) def[ D_PREV ] ) {
		if( def[ D_NAME ] == *sp ) {	/* found it */
			*sp = (intptr_t) def;
			flag = 1;
			return;
		}
	}
	*sp = 0;
	flag = 0;
}

void compile_to_def( void )
{
	cptr = (intptr_t) deftop;
}

void compile_to_buffer( void )
{
	cptr = cbuf;
}

/*
 * Send a C string to output.
 */

void put_c_string( char *s ) { 
	while( *s ) {
		*--sp = *s++;
		put(); 
	}
}

/*
 * Hex ouput for C debugging.
 */

void put_hex( unsigned x )
{ char buf[16]; (void) snprintf( buf, 16, "%x", x);  put_c_string( buf );}



void putd( void ) 
{ char buf[16]; (void) snprintf( buf, 16, "%d", (int)*sp++); put_c_string( buf );}
void putx( void ) 
{ char buf[16]; (void) snprintf( buf, 16, "%x", (unsigned)*sp++);  put_c_string( buf );}
void putf( void ) 
{ char buf[100]; (void) snprintf( buf, 100, "%g", (double) *(fcell*)sp++ ); put_c_string( buf );}

void depth( void ) { cell w = stack - sp + STACK_DIM; *--sp = w; }

void nl( void ) { put_c_string( "\n" ); }
void space( void ) { put_c_string( " " ); }

/*
 * System interface
 */

void delay( void ) { /* stub */ }

/*
 * fussy interpreter for debugging
 */

static int befussy;

void fussy( void ) 
{
	befussy = 1;
	while( befussy ) {
		if( sp > stack + STACK_DIM ) {
			put_c_string( "Stack underflow!\n" );
			interrupt( 1 );
		}
		if( sp < stack ) {
			put_c_string( "Stack overflow!\n" );
			interrupt( 1 );
		}
		if( rsp > rstack + RSTACK_DIM ) {
			put_c_string( "Return stack underflow!\n" );
			interrupt( 1 );
		}
		if( rsp < rstack ) {
			put_c_string( "Return stack overflow!\n" );
			interrupt( 1 );
		}
		(*(prim *)(intptr_t)*(cell *)(intptr_t)(*++lc))();
	}
}

void fast( void ) { befussy = 0; }

void ifelse( void )
{
	cell *l = ++lc;
	if( !flag ) return;		/* just skip next word */
	(*(prim *)(intptr_t)*(cell *)(intptr_t)(*l))();  /* interpret next word */
	if( lc != l ) {			/* some sort of branch happened */
		if( interpret == (prim *)(intptr_t)*(cell *)(intptr_t)(*l)) {		/* if the branch was a call */
			*rsp = (cell)(intptr_t)((cell *)(intptr_t)(*rsp) + 1);			/* do the skip a level up */
		}   /* if it wasn't a call, don't worry, we're done */
		return;
	}
	lc += 1;	/* skip the else word */
}

/*
 * $Log$
 * Revision 1.9  2010-07-12 16:45:39  jpd
 * Improve array construction tools.
 * Fix casts in "float" and "fix".
 *
 * Revision 1.8  2010-06-08 18:57:41  jpd
 * Faults and user interrupts now work on SAM7A3
 *
 * Revision 1.7  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.6  2009-06-01 16:54:19  jpd
 * Installation instructions.
 * Fix line editing, allow external reset.
 *
 * Revision 1.5  2009-03-26 01:26:22  jpd
 * Better factoring.
 *
 * Revision 1.4  2009-03-11 03:43:59  jpd
 * Fix abort handling.
 *
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


