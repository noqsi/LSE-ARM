/*
 * bit-bang write of AD5308 et al. DACs
 */

#ifndef AD53BB_H
#define AD53BB_H 1

#include <stdint.h>

/*
 * pio points to the PIO controller base address, either
 * PIOA or PIOB.
 *
 * sclk, sync, and din are single bit masks specifying the port bit
 * for each control signal.
 */

struct ad53 {
	struct pio *pio;
	uint32_t sclk, sync, din;
};

void ad53_write( struct ad53 *p, uint16_t v );
void ad53_primitives( void );

#endif /* ndef AD53BB_H */
