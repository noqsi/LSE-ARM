/*
 * bit-bang read of Analog Devices "Pulsar" ADC chips
 */


#ifndef PULSAR_H
#define PULSAR_H 1

#include <stdint.h>

#define PULSAR_TIMEOUT 0x80000000	/* return this on timeout */

/*
 * pio points to the PIO controller base address, either
 * PIOA or PIOB.
 *
 * sck, cnv, and sdo are single bit masks specifying the port bit
 * for each control signal.
 *
 * bits is the number of bits to read.
 *
 * snx is the (extended) sign bit. It should have its most (33-bits) MSB set.
 * For bits==16, it's thus 0xffff8000 and for bits==18 it's 0xfffe0000.
 */

struct pulsar {
	struct pio *pio;
	uint32_t sck, cnv, sdo;
	unsigned bits;
	uint32_t snx;	/* sign extension bits */
};

int32_t pulsar_read( struct pulsar *p );
void pulsar_primitives( void );


#endif /* ndef PULSAR_H */
