/* $Id$ */

/*
Peripheral ID's for clock and interrupt management. Used to identify
the peripherals to the PMC and AIC.
*/

/* 
The AIC gloats that it owns five ID's, named here by their interrupts.
*/

#define FIQ_ID	0
#define IRQ0_ID	28
#define IRQ1_ID	29
#define IRQ2_ID	30
#define IRQ3_ID	31

/* And here are the ones that have to live on one ID apiece */

#define SYSC_ID	1
#define PIOA_ID 2
#define PIOB_ID 3
#define CAN0_ID	4
#define CAN1_ID	5
#define US0_ID	6	/* USART 0 */
#define US1_ID	7	/* USART 1 */
#define US2_ID	8
#define MCI_ID	9
#define TWI_ID	10
#define SPIO_ID	11
#define SPI1_ID	12
#define SSC0_ID	13
#define SSC1_ID	14
#define	TC0_ID	15
#define TC1_ID	16
#define TC2_ID	17
#define TC3_ID	18
#define	TC4_ID	19
#define TC5_ID	20
#define TC6_ID	21
#define TC7_ID	22
#define TC8_ID	23
#define ADC0_ID	24
#define ADC1_ID	25
#define PWMC_ID	26
#define UDP_ID	27


/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */

