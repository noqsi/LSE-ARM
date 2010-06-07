/* $Id$ */

/*
Peripheral ID's for clock and interrupt management. Used to identify
the peripherals to the PMC and AIC.
*/

/* 
The AIC gloats that it owns three ID's, named here by their interrupts.
*/

#define FIQ_ID	0
#define IRQ0_ID	30
#define IRQ1_ID	31

/* And here are the ones that have to live on one ID apiece */

#define SYSC_ID	1
#define PIOA_ID 2
#define PIOB_ID 3
#define SPI0_ID	4
#define SPI1_ID	5
#define US0_ID	6	/* USART 0 */
#define US1_ID	7	/* USART 1 */
#define SSC_ID	8
#define TWI_ID	9
#define PWMC_ID	10
#define UDP_ID	11
#define TC0_ID	12	/* Timer/Counter 0 */
#define TC1_ID	13	/* Timer/Counter 1 */
#define TC2_ID	14	/* Timer/Counter 2 */
#define	CAN_ID	15
#define EMAC_ID	16
#define ADC_ID	17

/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */

