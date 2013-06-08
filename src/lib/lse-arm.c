/* $Id$ */

/*
    Copyright 2004, 2005, 2006, 2009, 2010, 2011, 2013 
    John P. Doty and Matthew P. Wampler-Doty

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
#include <setjmp.h>

#include "ROM-dict.h"

cell stack[STACK_DIM+STACK_PAD], rstack[RSTACK_DIM], compile_buf[CBUF_DIM];
//	defmem[DEFMEM_DIM], constmem[CONSTMEM_DIM];

cell	*sp,		/* top active item */
        *rsp,		/* top active return stack item */
        *lc,		/* interpreter's location counter */
	*deftop,	/* top of definition dictionary */
	*deflast,	/* last entry in definition dictionary */
	*constop,	/* top of constant dictionary */
	*constlast,	/* last entry of constant dictionary */
	*defend,	/* end of definition area */
	*constend;	/* end of constant area */
		
int flag;		/* the flag register */

cell cptr;		/* compile pointer */
cell cbuf;		/* loc of compile buffer */

jmp_buf rexeq;

/*
 * The bootstrap interpreter starts at the start of the compile buffer.
 * It returns to the bootstrap compiler upon return stack underflow.
 */

void bootinterpreter( void ) 
{ 
	lc = ((cell *)(intptr_t) cbuf) - 1;
	rsp = rstack + RSTACK_DIM;
	
	while(  rsp <= rstack + RSTACK_DIM ) {
		cell *dp = (cell *)(intptr_t)(*++lc);
		prim *pp = (prim *)(intptr_t)*dp;
		(*pp)();
	}
}

/*
 * The normal fast indirect threaded interpreter loop.
 */
 
void interpreter( void ) { for(;;) (*(prim *)(intptr_t)*(cell *)(intptr_t)(*++lc))(); }

/* Get back to top level after an interrupt or fault */

void interrupt( int sig )
{
	longjmp( rexeq, sig );
}

void abort( void )
{
	interrupt( 1 );
}

/*
 * Special primitive to start LSE once bootstrap has gone far enough.
 */

void xeq( void )
{
	cell * volatile savedlc = ((cell *) (intptr_t) *(lc+1)) + D_BODY - 1;
	int sig;
	sig = setjmp( rexeq );

	switch( sig ) {
	/* here's where to have different actions for different aborts */
	case 0 : /* first time through */
		break;
	case 1 : put_c_string( "\nSoftware abort\n" );
		break;
	case 2 : put_c_string( "\nUndefined instruction\n" );
		break;
	case 3 : put_c_string( "\nSoftware interrupt\n" );
		break;
	case 4 : put_c_string( "\nMemory access error\n" );
		break;
	case 6 : put_c_string( "\nUncaught IRQ\n" );
		break;
	case 7 : put_c_string( "\nUncaught FIQ\n" );
		break;
	case 8 : put_c_string( "\nInterrupted\n" );
		break;
	default:
		put_c_string( "\nUnknown condition\n" );
		break;
	}
	
	if( sig ) primitive_io_abort();	/* reset input after restart */
	
	sp = stack + STACK_DIM;
	rsp = rstack + RSTACK_DIM;
 	lc = savedlc;
	fussy();		/* returns if fast interpreter requested */
	interpreter();					/* Never returns */
}

extern void *sbrk(intptr_t increment);

void setup_memory( void )
{
	cell free;
	extern cell end;
	
    	sp = stack + STACK_DIM;
    	rsp = rstack + RSTACK_DIM;
//	deflast = 0;
//	constlast = 0;
	cbuf = (cell) compile_buf;
	
	/* hack to estimate free mem, allocate to dictionaries */

	free = &free - &end - RESERVE;
	deftop = sbrk( sizeof(cell) * (&free - &end - RESERVE ));
	
	if( deftop == (void *) -1 ) {
		put_c_string( "\nDictionary memory allocation failed!\n" );
		for(;;);
	}
	
	defend = constop = deftop + free/2;
	constend = deftop + free;
}

/*
 * ascii_to_string takes a null terminated C string and turns it into
 * an LSE counted string temporary at the end of the constant dictionary.
 */

void ascii_to_string( char *str )
{
	cell c, *p = constop + D_DATA + 1;
	
	while(( c = *str++) ) *p++ = c;
	constop[ D_DATA ] = p - ( constop + D_DATA + 1 );
}

void ascii_to_literal( char *s )
{
	ascii_to_string( s );
	fixate_literal_string();
}

void build_primitive( prim p, char *name )
{
    ascii_to_literal( name );		/* leaves pointer to name on stack */
    make_def();				/* use it */
    deflast[ D_PRIM ] = (intptr_t) p;	/* pointer to function */
}

void build_named_constant( cell c, char *name )
{
	*--sp = c;
	ascii_to_literal( name );
	make_named_constant();
}

void build_primitives( void )
{
	// Most primitives are static in ROM, but these must be built
	// at initialization.
	
	build_named_constant( (cell) defend, "{DEFEND}" );
	build_named_constant( (cell) constend, "{CONSTEND}" );
}
    
void bootcompile( void )
{
	for( skip_space(); flag; skip_space() ) {
		get_name();
		find();
//		if( !flag ) {printf( "not found \n"); exit(1);}
if( !flag ) { writechar( 'Z' ); for(;;); }
		*--sp = (cell) &cptr; append();
	}
	ascii_to_literal( "exit" );
	find(); *--sp = (cell) &cptr ; append();
}

	
/*
 * bootstrap() reads a pseudofile of "pidgin LSE" commands and executes them.
 * These should build a compiler for the full language.
 * In this file:
 * Any line beginning with # is a comment.
 * Any line beginnng with : is a definition. The next token is its name.
 * Otherwise, the line is a command.
 */

void bootstrap( void )
{
	for( ;; ) {
		get();
		switch( *sp ) {
		case '#':
			drop();
			skip_to_nl();
			break;
		
		case ':':
			drop();
			get_name();
			make_def();
			compile_to_def();
			bootcompile();
			fixate_colon_def();
			break;
			
		default:
			unget();
			compile_to_buffer();
			bootcompile();
			bootinterpreter();
			break;
		}
	}		
 }  
 
 

void lse_init( void )
{
    setup_memory();
    build_primitives();
}


void lse_main( void )
{
    bootstrap();
}

/*
 * $Log$
 * Revision 1.10  2010-07-13 18:38:07  jpd
 * First draft of low level SPI driver.
 *
 * Revision 1.9  2010-06-10 17:53:07  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
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

