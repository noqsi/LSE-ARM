/* $Id$ */

#ifndef WDT_H
#define WDT_H 1

#include <stdint.h>

/* ARM SAM7X watchdog timer */

struct wdt {
	volatile uint32_t cr, mr, sr;
};

/* For now, just define enough to disable the thing */

#define WDDIS	0x8000

#define watchdog_disable() ( WDT->mr = WDDIS )

#endif /* ndef WDT_H */

/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */
