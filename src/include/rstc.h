/* $Id$ */

/* Reset Controller */

#ifndef RSTC_H
#define RSTC_H 1

#include <stdint.h>

struct rstc {
	volatile uint32_t cr, sr, mr;
};

#define RSTC_KEY 0xa5000000	/* key to validate write */

/* CR */

#define PROCRST		0x1
#define PERRST		0x4
#define EXTRST		0x8

/* SR */

#define URSTS		0x1
#define BODSTS	 	0x2
#define RSTTYP		0x700		/* mask to extract reset type */
#define NRSTL		0x10000
#define SRCMP		0x20000

/* MR */

#define URSTEN	0x1
#define URSTIEN	0x10
#define ERSTL(n) (((n)&0xf)<<8)		/* macro to make reset length field */
#define BODIEN	0x10000

#endif /* ndef RSTC_H */

/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2009-06-02 19:53:26  jpd
 * Allow external reset.
 *
 */


