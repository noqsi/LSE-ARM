/* $Id$ */

/*
    Copyright 2004, 2005, 2006, 2009, 2013
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


#include <stdint.h>

typedef int32_t cell;
typedef float fcell;

extern cell stack[], rstack[];

extern cell	*sp,		/* top active item */
        	*rsp,		/* top active return stack item */
       		*lc,		/* interpreter's location counter */
		*deftop,	/* top of definition dictionary */
		*deflast,   /* last entry in definition dictionary */
		*defend,	/* end of def area */
		*constop,   /* top of constant dictionary */
		*constlast, /* last entry of constant dictionary */
		*constend;	/* end of const area */
		
extern int flag;   /* the flag register */

extern cell cptr;		/* compile pointer */
extern cell cbuf;		/* loc of compile buffer */

extern cell FlowPrompt;		/* line prompt character */

/*
 * Offsets of fields within a dictionary entry.
 */
 
#define		D_PRIM  0			/* pointer to primitive func */
#define		D_PREV  1			/* pointer to previous dictionary entry */
#define		D_PRECEDENCE	2   /* precedence of this word in compilation */
#define		D_DATA  3			/* data field */

/*
 * Offsets of additional fields within definitions.
 */

#define		D_NAME			(D_DATA+0)  /* pointer to name */
#define		D_BODY			(D_DATA+1)  /* The contents */

/*
 *  Precedence values
 */

#define		DP_COMPILED		0   /* Normal compiled word */
#define		DP_IMMEDIATE	1   /* Execute instead of compiling */
#define		DP_SOONER		2   /* Execute before compiling previous word */

/*
 * These should eventually be configured based on host resources
 */
 
#define	STACK_DIM	64
#define STACK_PAD	4	/* To accomodate some underflow */
#define RSTACK_DIM	64
// #define	DEFMEM_DIM	3000
// #define CONSTMEM_DIM	3000
#define CBUF_DIM	160
#define STRINGBUF_DIM	200
#define RESERVE		1200	// Cells to reserve for C stack and heap

/*
 * Primitive functions
 */

typedef void prim(void);

prim
fetch, store, fetchp, storep, append, stringconstant, constant, variable, add,
subtract, multiply, divide, mod, neg, fadd, fsubtract, fmultiply,
fdivide, fmodd, fneg, and, or, xor, not, dupp, swap, drop, pick, rot,
equal, greater, less, equal0, greater0, less0, fequal, fgreater,
fless, fequal0, fgreater0, fless0, setflag, clearflag, exitrue,
exitfalse, exitword, iftrue, iffalse, repeat, repeatrue, s2r, r2s,
rcopy, rdrop, put, unget, get, get_nowait, skip_space, skip_to_nl, get_token,
fixate_literal_string, get_name, make_def, interpret, fixate_colon_def,
make_named_constant, make_array, make_constant, find, compile_to_def,
compile_to_buffer, literal, noop, cells, putd, putx, putf, depth, nl, space,
put_string, get_stdin, get_stdout, get_stderr, get_in, put_in, get_out,
put_out, flush, eofq, ioerrq, ToNumber, HexNumber, delay, sysexit, ffloat, fix,
openf, closef, iterate_init, iterate, anditerate, count, fussy, fast, ifelse,
argc, arg, now, scan, jfalse, sprint1, openfd, named_constant, xeq;

void build_primitive( prim p, char *name );
void build_named_constant( cell c, char *name );

/* strings.c stuff */
char *cstring( char *b, int n );

/* other inter-source links */

void interrupt( int sig ) __attribute__ ((noreturn));
void put_c_string( char *s );
void copy_static( void );
void put_hex( unsigned x );
void primitive_io_abort( void );
int char_ready( void );
void writechar( char c );

/*
 * $Log$
 * Revision 1.3  2010-07-13 18:38:05  jpd
 * First draft of low level SPI driver.
 *
 * Revision 1.2  2010-06-08 18:57:41  jpd
 * Faults and user interrupts now work on SAM7A3
 *
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.6  2010-06-04 18:13:38  jpd
 * Update build for multiple targets.
 *
 * Revision 1.5  2009-06-01 16:54:19  jpd
 * Installation instructions.
 * Fix line editing, allow external reset.
 *
 * Revision 1.4  2009-03-26 01:26:22  jpd
 * Better factoring.
 *
 * Revision 1.3  2009-03-11 03:43:59  jpd
 * Fix abort handling.
 *
 * Revision 1.2  2009-03-11 02:19:42  jpd
 * It compiles, executes.
 * Prompt doesn't work.
 * OS hooks need removal.
 *
 * Revision 1.1.1.1  2009-03-10 19:08:44  jpd
 * Initial sources
 *
 */

