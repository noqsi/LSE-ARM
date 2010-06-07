/* $Id$ */

/*
 * SAM7X PIO controller
 */


#ifndef PIO_H
#define PIO_H 1

#include <stdint.h>

struct pio {
	volatile uint32_t per, pdr, psr, res0, 
		oer, odr, osr, res1, ifer, ifdr, ifsr, res2, 
		sodr, codr, odsr, pdsr, ier, idr, imr, isr,
		mder, mddr, mdsr, res3, pudr, puer, pusr, res4,
		asr, bsr, absr, gap[9], ower, owdr, owsr;
};

#endif /* ndef PIO_H */

/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */
