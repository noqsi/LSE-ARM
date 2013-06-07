/* Atmel ADC cell */

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

struct adc {
	volatile uint32_t cr, mr, res[2], cher, chdr, chsr, sr, lcdr, ier,
		idr, imr, cdr[8];
};

/* CR constants */

#define SWRST 1
#define START 2

/* MR */

#define TRGEN 1
#define TRGSEL_SHIFT 1
#define LOWRES 0x10
#define SLEEP 0x20
#define PRESCAL_SHIFT 8
#define STARTUP_SHIFT 16
#define SHTIM_SHIFT 24

/* SR, IER, IDR, IMR */

#define EOC(n) (1<<(n))
#define OVRE(n) (1<<((n)+8))
#define DRDY 0x10000
#define GOVRE 0x20000
#define ENDRX 0x40000
#define RXBUFF 0x80000 

#endif /* ndef ADC_H */
