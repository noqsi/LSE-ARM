/* Memory Controller */

#ifndef MC_H
#define MC_H 1

#include <stdint.h>

struct mc {
	volatile uint32_t rcr, asr, aasr, res, puia[16], pup, puer, gap[2],
		fmr, fcr, fsr; };

/* being lazy, right now all I'm coding is the FWS field of FMR */

#define FWAIT1 0x000
#define FWAIT2 0x100
#define FWAIT3 0x200
#define FWAIT4 0x300

#endif /* ndef MC_H */
