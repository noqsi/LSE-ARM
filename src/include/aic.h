/* $Id$ */

/* Power Management Controller */

#ifndef AIC_H
#define AIC_H 1

#include <stdint.h>

struct aic {
	volatile uint32_t smr[32], svr[32], ivr, fvr, isr, ipr,
		imr, cisr, res1[2], iecr, idcr, iccr, iscr, eoicr,
		spu, dcr, res2, ffer, ffdr, ffsr;
};	

/* SMR constants */

#define TRIGGER_LEVEL	0
#define TRIGGER_EDGE	0x20
#define TRIGGER_HIGH	0x40
#define TRIGGER_LOW	0
#define PRIORITY_BITS	0x7

/* CISR constants */

#define NFIQ		0x1
#define NIRQ		0x2

/* DCR constants */

#define PROT		0x1
#define GMSK		0x2

#endif /* ndef AIC_H */

/*
 * $Log$
 * Revision 1.1  2010-06-07 18:33:44  jpd
 * AIC driver.
 *
 */


