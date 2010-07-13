/* $Id$ */

/*** Memory Layout for the SAM7A3 Board ***/

#ifndef MEMORY_H
#define MEMORY_H 1

#ifndef bit
#define bit(n) (1<<(n))
#endif

/* ARM SAM7X memory layout */

#define INTERNAL_MEMORY ((void*)0)
#define INTERNAL_PERIPHERALS ((void*)0xf0000000)

/* Memory blocks */

#define BOOT_MEMORY (INTERNAL_MEMORY + 0)
#define INTERNAL_FLASH (INTERNAL_MEMORY + 0x00100000)
#define INTERNAL_SRAM (INTERNAL_MEMORY + 0x00200000)

/* Peripherals */

	/* Controller Area Network */
#define CAN0 ((struct can*) (INTERNAL_PERIPHERALS + 0xff80000))
#define CAN1 ((struct can*) (INTERNAL_PERIPHERALS + 0xff80000))


	/* Timer Counter */
#define TC012 ((struct tc*) (INTERNAL_PERIPHERALS + 0xffa0000))
#define TC345 ((struct tc*) (INTERNAL_PERIPHERALS + 0xffa4000))
#define TC678 ((struct tc*) (INTERNAL_PERIPHERALS + 0xffa8000))

	/* Multimedia Card Interface */
#define MCI ((struct mci*) (INTERNAL_PERIPHERALS + 0xffac000))

	/* USB Device Port */
#define UDP ((struct udp*) (INTERNAL_PERIPHERALS + 0xffb0000))

	/* Two wire interface */
#define TWI ((struct two_wire_interface*) (INTERNAL_PERIPHERALS + 0xffb8000))

	/* Universal Synchronous Asynchronous Receiver Transceivers */
#define USART ((struct usart*) (INTERNAL_PERIPHERALS + 0xffc0000))
#define USART0 ((struct usart*) (INTERNAL_PERIPHERALS + 0xffc0000))
#define USART1 ((struct usart*) (INTERNAL_PERIPHERALS + 0xffc4000))

	/* Pulse Width Modulator Controller */
#define PWM ((struct pwmc*) (INTERNAL_PERIPHERALS + 0xffcc000))

	/* Synchronous Serial Controllers */
#define SSC0 ((struct ssc*) (INTERNAL_PERIPHERALS + 0xffd0000))
#define SSC1 ((struct ssc*) (INTERNAL_PERIPHERALS + 0xffd4000))

	/* Analog-to-Digital Converter */
#define ADC0 ((struct adc*) (INTERNAL_PERIPHERALS + 0xffd8000))
#define ADC1 ((struct adc*) (INTERNAL_PERIPHERALS + 0xffdC000))

	/* Serial Peripheral Interface */
#define SPI0 ((struct spi*) (INTERNAL_PERIPHERALS + 0xffe0000))
#define SPI1 ((struct spi*) (INTERNAL_PERIPHERALS + 0xffe4000))

	/* System Controller */
#define SYSC ((INTERNAL_PERIPHERALS + 0xffff000))

/* System Controller */

	/* Advanced Interrupt Controller */
#define AIC ((struct aic*) (SYSC + 0))

	/* Debug Unit */
#define DBGU ((struct usart*) (SYSC + 0x200))	/* Close enough to a USART */

	/* PIO Controllers */
#define PIOA ((struct pio*) (SYSC + 0x400))
#define PIOB ((struct pio*) (SYSC + 0x600))

	/* Power Management Controller */
#define PMC ((struct pmc*) (SYSC + 0xc00))

	/* Reset Controller */
#define RSTC ((struct rstc*) (SYSC + 0xd00))

	/* Shut-down Controller */
#define SHDWC ((struct rtt*) (SYSC + 0xd10))

	/* Real-time Timer */
#define RTT ((struct rtt*) (SYSC + 0xd20))

	/* Periodic Interval Timer */
#define PIT ((struct pit*) (SYSC + 0xd30))

	/* Watchdog Timer */
#define WDT ((struct wdt*) (SYSC + 0xd40))

	/* General Purpose Backup Registers */
#define GPBR ((struct gpbr*) (SYSC + 0xd50))

	/* Memory Controller */
#define MC ((struct mc*) (SYSC + 0xf00))

#endif /* ndef MEMORY_H */

/*
 * $Log$
 * Revision 1.2  2010-07-13 18:38:06  jpd
 * First draft of low level SPI driver.
 *
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2010-06-04 18:34:57  jpd
 * More multibuild stuff.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */

