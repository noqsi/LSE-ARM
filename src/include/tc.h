/* Timer/counter */

#ifndef TC_H
#define TC_H 1

#include <stdint.h>

struct tc_channel {
	volatile uint32_t ccr, cmr, res1, res2, cv, ra, rb, rc, sr,
		ier, idr, imr, res3[4];
};

struct tc {
	struct tc_channel channel[3];
	volatile uint32_t bcr, bmr;
};

/* bcr */

#define SYNC 0x1

/* bmr */

#define TCXCS(n,code) ((code)<<(2*(n)))

/* ccr */

#define CLKEN 0x1
#define CLKDIS 0x2
#define SWTRIG 0x4

/* cmr */

#define TIMER_CLOCK1 0	/* Clock selection bits */
#define TIMER_CLOCK2 1
#define TIMER_CLOCK3 2
#define TIMER_CLOCK4 3
#define TIMER_CLOCK5 4
#define XC0 5
#define XC1 6
#define XC2 7

#define CLKI 0x8

#define ANDXC0 0x10	/* burst selection */
#define ANDXC1 0x20
#define ANDXC2 0x30

#define LDBSTOP 0x40
#define LDBDIS 0x80

#define ETRG_RISING 0x100	/* external trigger edge */
#define ETRG_FALLING 0x200

#define ETRG_TIOB 0		/* external trigger source */
#define ETRG_TIOA 0x400

#define CPCTRG 0x4000

#define LDRA_RISING 0x10000	/* Edge of TIOA loading RA */
#define LDRA_FALLING 0x20000

#define LDRB_RISING 0x40000
#define LDRB_FALLING 0x80000

/* Wave mode left for later */

/* sr, ier, idr, imr */

#define COVFS 0x1	/* counter overflow */
#define LOVRS 0x2	/* load overrun */
#define CPAS 0x4	/* RA compare */
#define CPBS 0x8	/* RB compare */
#define CPCS 0x10	/* RC compare */
#define LDRAS 0x20	/* Load RA */
#define LDRBS 0x40	/* Load RB */
#define ETRGS 0x80	/* External trigger */
#define CLKSTA 0x10000	/* sr only */
#define MTIOA 0x20000	/* sr only */
#define MTIOB 0x40000	/* sr only */

#endif /* ndef TC_H */
