#ifndef PWM_H
#define PWM_H 1

#include <stdint.h>

struct pwm_channel {
	volatile uint32_t cmr, cdty, cprd, ccnt, cupd, reserved[3];
};


struct pwmc {
	volatile uint32_t mr, ena, dis, sr, ier, idr, imr, isr,
		reserved[120];
	struct pwm_channel channel[8];
};

/* mr */

#define PREB_MASK	0x0f000000
#define PREB_SHIFT	24
#define DIVB_MASK	0x00ff0000
#define DIVB_SHIFT	16
#define PREA_MASK	0x00000f00
#define PREA_SHIFT	8
#define DIVA_MASK	0x000000ff
#define DIVA_SHIFT	0

/* cmr */

#define CPD		0x400
#define CPOL		0x200
#define CALG		0x100
#define	CPRE_MASK	0x00f
#define CPRE_SHIFT	0

#endif /* ndef PWM_H */
