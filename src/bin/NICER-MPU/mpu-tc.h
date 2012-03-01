/* Timer/counter functions for MPU */

#ifndef MPU_TC_H
#define MPU_TC_H 1

#include <stdint.h>

uint32_t rawtime( uint16_t * raw16ms );
void pps_start( void );
void tc_primitives( void );

#endif /* MPU_TC_H */
