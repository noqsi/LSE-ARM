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
#include <setjmp.h>

cell stack[STACK_DIM], rstack[RSTACK_DIM], compile_buf[CBUF_DIM],
	defmem[DEFMEM_DIM], constmem[CONSTMEM_DIM];

cell	*sp,		/* top active item */
        *rsp,		/* top active return stack item */
        *lc,		/* interpreter's location counter */
	*deftop,	/* top of definition dictionary */
	*deflast,	/* last entry in definition dictionary */
	*constop,	/* top of constant dictionary */
	*constlast;	/* last entry of constant dictionary */
		
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

/*
 * Special primitive to start LSE once bootstrap has gone far enough.
 */

void xeq( void )
{
	cell * volatile savedlc = ((cell *) (intptr_t) *(lc+1)) + D_BODY - 1;

	switch( setjmp( rexeq )) {
	/* here's where to have different actions for different aborts */
	
	case 1 : put_c_string( "Aborted!\n" );
		break;
	default:
		break;
	}
	
	sp = stack + STACK_DIM;
	rsp = rstack + RSTACK_DIM;
 	lc = savedlc;
	fussy();		/* returns if fast interpreter requested */
	interpreter();					/* Never returns */
}

void setup_memory( void )
{
    	sp = stack + STACK_DIM;
    	rsp = rstack + RSTACK_DIM;
	deftop = defmem;
	deflast = 0;
	constop = constmem;
	constlast = 0;
	cbuf = (cell) compile_buf;
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
    build_primitive( xeq, "//XEQ" );
    build_primitive( fixate_literal_string, ">string<" );
    build_primitive( make_def, ">def<" );
    build_primitive( make_constant, ">constant<" );
    build_primitive( make_named_constant, ">named_constant<" );
    build_primitive( make_array, ">array<" );
    build_primitive( equal, "=" );
    build_primitive( fequal, "=." );
    build_primitive( greater, ">" );
    build_primitive( fgreater, ">." );
    build_primitive( less, "<" );
    build_primitive( fless, "<." );
    build_primitive( equal0, "0=" );
    build_primitive( fequal0, "0=." );
    build_primitive( greater0, "0>" );
    build_primitive( fgreater0, "0>." );
    build_primitive( less0, "0<" );
    build_primitive( fless0, "0<." );
    build_primitive( setflag, "true" );
    build_primitive( clearflag, "false" );
    build_primitive( iffalse, "ifnot" );
    build_primitive( iftrue, "if" );
    build_primitive( fetch, "@" );
    build_primitive( fetchp, "{@}" );
    build_primitive( store, "!" );
    build_primitive( storep, "{!}" );
    build_primitive( append, "@!+" );
    build_primitive( scan, "@@+" );
    build_primitive( add, "+" );
    build_primitive( fadd, "+." );
    build_primitive( subtract, "-" );
    build_primitive( fsubtract, "-." );
    build_primitive( multiply, "*" );
    build_primitive( fmultiply, "*." );
    build_primitive( divide, "/" );
    build_primitive( fdivide, "/." );
    build_primitive( neg, "neg" );
    build_primitive( fneg, "neg." );
    build_primitive( mod, "%" );
    build_primitive( fmodd, "%." );
    build_primitive( and, "&" );
    build_primitive( or, "|" );
    build_primitive( xor, "^" );
    build_primitive( not, "~" );
    build_primitive( ffloat, "float" );
    build_primitive( fix, "fix" );
    build_primitive( dupp, "dup" );
    build_primitive( swap, "swap" );
    build_primitive( drop, "drop" );
    build_primitive( pick, "pick" );
    build_primitive( rot, "rot" );
    build_primitive( exitword, "exit" );
    build_primitive( exitfalse, "&&" );
    build_primitive( exitrue, "||" );
    build_primitive( jfalse, "|jmp" );
    build_primitive( repeat, "repeat" );
    build_primitive( repeatrue, "&repeat" );
    build_primitive( iterate_init, "iterate[" );
    build_primitive( iterate, "]iterate" );
    build_primitive( anditerate, "]&iterate" );
    build_primitive( count, "count" );	
    build_primitive( r2s, "r>" );
    build_primitive( s2r, ">r" );
    build_primitive( rcopy, "r@" );
    build_primitive( rdrop, "rdrop" );
    build_primitive( literal, "'" );
    build_primitive( noop, "{}" );
    build_primitive( cells, "[]" );
    build_primitive( get, "get" );
    build_primitive( put, "put" );
    build_primitive( unget, "unget" );
    build_primitive( skip_space, "skip_space" );
    build_primitive( skip_to_nl, "skip>nl" );
    build_primitive( get_token, "<token?" );
    build_primitive( get_name, "<name?" );
    build_primitive( find, "find" );
    build_primitive( putd, "," );
    build_primitive( putx, ",h" );
    build_primitive( putf, ",." );
    build_primitive( depth, "depth" );
    build_primitive( nl, "nl" );
    build_primitive( space, "sp" );
    build_primitive( put_string, ",t" );
    build_primitive( ToNumber, ">number?" );
    build_primitive( HexNumber, ">hex?" );
    build_primitive( delay, "usec" );
    build_primitive( fussy, "fussy" );
    build_primitive( fast, "fast" );
    build_primitive( abort, "abort" );
    build_primitive( ifelse, "ifelse" );

/*
 * VM/dictionary "registers":
 * These constants point to variables used by primitives. Note that these are
 * not necessarily cells, so you must use {@} and {!} to load and store them.
 */
 
    build_named_constant( (cell) &stack, "{stack}" );
    build_named_constant( (cell) &sp, "{sp}" );
    build_named_constant( (cell) &rstack, "{rstack}" );
    build_named_constant( (cell) &rsp, "{rsp}" );
    build_named_constant( (cell) &lc, "{lc}" );
    build_named_constant( (cell) &flag, "{flag}" );
        
    build_named_constant( (cell) &deftop, "{deftop}" );
    build_named_constant( (cell) &deflast, "{deflast}" );
    build_named_constant( (cell) &constop, "{constop}" );
    build_named_constant( (cell) &constlast, "{constlast}" );
        
/*
 * VM/dictionary constants:
 */
	
    build_named_constant( STACK_DIM, "{STACK}" );
    build_named_constant( RSTACK_DIM, "{RSTACK}" );
    build_named_constant( DEFMEM_DIM, "{DEFMEM}" );
    build_named_constant( CONSTMEM_DIM, "{CONSTMEM}" );
        
    build_named_constant( D_PREV, "{PREV}" );
    build_named_constant( D_DATA, "{DATA}" );
    build_named_constant( D_PRECEDENCE, "{PRECEDENCE}" );
    build_named_constant( D_NAME, "{NAME}" );
    build_named_constant( D_BODY, "{BODY}" );
        
    build_named_constant( DP_COMPILED, "{COMPILED}" );
    build_named_constant( DP_IMMEDIATE, "{IMMEDIATE}" );
    build_named_constant( DP_SOONER, "{SOONER}" );

/* Compiler constants and pointers to compiler variables */
	
    build_named_constant( (cell) &cptr, "cptr" );
    build_named_constant( (cell) &cbuf, "cbuf" );
    build_named_constant( CBUF_DIM, "[CBUF]" );
    build_named_constant( (cell) interpret, "interpret" );
    build_named_constant( 0, "0" );
    build_named_constant( 1, "1" );
}
    
void bootcompile( void )
{
	for( skip_space(); flag; skip_space() ) {
		get_name();
		find();
//		if( !flag ) {printf( "not found \n"); exit(1);}
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
 
 

void lse_main( void )
{
    setup_memory();
    build_primitives();
    bootstrap();
}

/*
 * $Log$
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

