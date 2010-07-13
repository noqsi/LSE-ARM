/* $Id$ */

#ifndef PIT_H
#define PIT_H 1


#include <stdint.h>

struct pit {
	volatile uint32_t mr, sr, pivr, piir;
};


/* mr */

#define PIV_MASK	0x000fffff
#define PITEN		0x01000000
#define PITIEN		0x02000000

/* sr */

#define PITS		1

/* extract fields from pivr, piir */

#define PICNT(x)	((x)>>20)
#define CPIV(x)		((x)&PIV_MASK)

/* macro to extract the tick frequency */

#define GET_TICK_HZ(pit,mck_hz)	((mck_hz)/((pit->mr & PIV_MASK)+1)/16)

#endif /* ndef PIT_H */

/*
 * $Log$
 * Revision 1.2  2010-07-13 18:38:06  jpd
 * First draft of low level SPI driver.
 *
 * Revision 1.1  2010-06-10 17:53:07  jpd
 * Completed interrupt infrastructure.
 * Periodic timer interrupt working on SAM7A3.
 * Commented out some unnecessary definitions.
 * Added ability to display free memory.
 *
 */


	
