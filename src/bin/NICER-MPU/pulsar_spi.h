/*
 * Read Analog Devices "Pulsar" ADC chips via SPI device
 */


#ifndef PULSAR_SPI_H
#define PULSAR_SPI_H 1

#include <stdint.h>
#include <spi.h>

void pulsar_spi_init( struct spi *spi );
int16_t pulsar_spi_read( struct spi *spi );
void pulsar_spi_primitives( void );

#endif /* ndef PULSAR_SPI_H */
