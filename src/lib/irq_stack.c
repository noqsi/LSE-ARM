/* $Id$ */

/* Provide a default IRQ stack */

#define DIM 100

static int stack[ DIM ];
int * const irq_stack_top = stack + DIM - 1;

/*
 * $Log$
 * Revision 1.1  2010-06-08 20:28:17  jpd
 * Default IRQ stack allocation.
 *
 */
