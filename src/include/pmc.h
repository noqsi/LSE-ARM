/* $Id$ */

/* Power Management Controller */

#ifndef PMC_H
#define PMC_H 1

#include <stdint.h>

struct pmc {
	volatile uint32_t scer, scdr, scsr, res0, pcer, pcdr, pcsr, res1,
		mor, mcfr, res2, pllr, mckr, res3, res4, res5, pck[3],
		gap[5], ier, idr, sr, imr;
};

/* SCER, SCDR, SCSR */

#define ENB_PCK		0x1
#define ENB_UDP		0x80
#define ENB_PCK0	0x100
#define ENB_PCK1	0x200
#define ENB_PCK2	0x400

/* MOR */

#define MOSCEN		0x1
#define OSCBYPASS 	0x2
#define OSCOUNT(n)	((n)<<8)

/* MCFR */

#define MAINF	0xffff		/* mask to extract osc frequency */
#define MAINRDY	0x80000

/* PLLR */

#define DIV(n)		(n)
#define PLLCOUNT(n)	((n)<<8)
#define OUT(n)		((n)<<14)
#define MUL(n)		((n)<<16)
#define USBDIV(n)	((n)<<28)

/* MCKR and MCK[] */

#define CSS_SLOW	0
#define CSS_MAIN	1
#define CSS_PLL		3
#define PRES(n)		((n)<<2)

/* IER, IDR, SR, IMR */

#define MOSCS		0x1
#define LOCK		0x4
#define MCKRDY		0x8
#define PCKRDY(n)	(1<<((n)+8)

#endif /* ndef PMC_H */

/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */


