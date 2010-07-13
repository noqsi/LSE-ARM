/* $Id$ */

#ifndef SPI_H
#define SPI_H 1


#include <stdint.h>

struct spi {
	volatile uint32_t cr, mr, rdr, tdr, sr, ier, idr, imr, res[4], csr[4];
};

#ifndef bit
#define bit(n) (1<<(n))
#endif

/* cr */

#define SPIEN		bit(0)
#define SPIDIS		bit(1)
#define SWRST		bit(7)
#define LASTXFER	bit(24)

/* mr */

#define MSTR		bit(0)
#define PS		bit(1)
#define PSDEC		bit(2)
#define MODFDIS		bit(4)
#define LLB		bit(7)
#define MAKE_PCS(x)	(((x)&0xf)<<16)
#define GET_PCS(x)	(((x)>>16)&0xf)
#define MAKE_DLYBCS(x)	(((x)&0xff)<<24)
#define GET_DLYBCS(x)	(((x)>>24)&0xff)

/* rdr */

#define GET_RD(x)	((x)&0xffff)

/* tdr */

#define MAKE_TD(x)	((x)&0xffff)

/* sr, ier, idr, imr */

#define RDRF		bit(0)
#define TDRE		bit(1)
#define	MODF		bit(2)
#define OVRES		bit(3)
#define ENDRX		bit(4)
#define ENDTX		bit(5)
#define RXBUFF		bit(6)
#define TXBUFE		bit(7)
#define NSSR		bit(8)
#define TXEMPTY		bit(9)
#define SPIENS		bit(16)

/* csr */

#define CPOL		bit(0)
#define NCPHA		bit(1)
#define SPI_MODE(x)	((((x)>>1)&1)|(((^(x))&1)<<1))	/* get CPOL, NCPHA from "mode" */
#define CSAAT		bit(3)
#define MAKE_BITS(x)	(((x)-8)<<4)
#define GET_BITS(x)	((((x)>>4)&0xf)+8)
#define MAKE_SCBR(x)	(((x)&0xff)<<8)
#define GET_SCBR(x)	(((x)>>8)&0xff)
#define MAKE_DLYBS(x)	(((x)&0xff)<<16)
#define GET_DLYBS(x)	(((x)>>16)&0xff)
#define MAKE_DLYBCT(x)	(((x)&0xff)<<24)
#define GET_DLYBCT(x)	(((x)>>24)&0xff)

#endif /* ndef SPI_H */

/*
 * $Log$
 * Revision 1.1  2010-07-13 18:38:06  jpd
 * First draft of low level SPI driver.
 *
 */


	
