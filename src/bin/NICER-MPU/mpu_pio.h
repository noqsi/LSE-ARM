/* PIO bits for the MPU */

#ifndef MPU_PIO_H
#define MPU_PIO_H 1

/* Port B bits */

#define CHAIN_ADDR	0x7
#define SELECT_FAST	(1<<3)
#define SLOW_HOLDn	(1<<4)
#define SLOW_TRIGn	(1<<5)
#define FAST_HOLDn	(1<<6)
#define FAST_TRIGn	(1<<7)
#define CRESET		(1<<8)
#define TCLK0		(1<<9)
#define TIOA		0x15555000
#define ADCIN		0x2aaa8000

#define B_BBIN	(SLOW_HOLDn|FAST_HOLDn|SLOW_TRIGn|FAST_TRIGn)
#define B_BBOUT (CHAIN_ADDR|SELECT_FAST|CRESET)
#define B_PERIPH_A (TCLK0|TIOA)
#define B_PERIPH_B 0

#endif /* ndef MPU_PIO_H */
