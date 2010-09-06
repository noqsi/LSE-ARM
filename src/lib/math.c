/* $Id$ */

#include "lse-arm.h"
#include <math.h>

static void sin_( void ) { *(fcell *)sp = sinf(*(fcell *)sp); }
static void cos_( void ) { *(fcell *)sp = cosf(*(fcell *)sp); }
static void sqrt_( void ) { *(fcell *)sp = sqrtf(*(fcell *)sp); }

void math_primitives( void )
{
	build_primitive( sin_, "sin." );
	build_primitive( cos_, "cos." );
	build_primitive( sqrt_, "sqrt." );
	
}

/* 
 * $Log$
 * Revision 1.1  2010-09-06 20:30:26  jpd
 * Math functions.
 *
 */
