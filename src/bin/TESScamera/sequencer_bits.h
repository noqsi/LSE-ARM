/* $Id$ */

/* Mapping of sequencer control port to port B on the ARM chip */

#define SEQ_DATA	0x0003ffff	/* Data bits */
#define SEQ_ADDR	0x003c0000	/* Address bits */
#define SEQ_ADDR_SHIFT	18		/* Amount to shift address left */
#define	SEQ_RNW		0x00400000	/* Read not Write */
#define SEQ_STR		0x00800000	/* Strobe */
#define SEQ_ACK		0x01000000	/* Transfer Acknowledge */
#define SEQ_CMD_FIFO_FULL	0x02000000
#define SEQ_RESET	0x04000000

/* Addresses */

#define	SEQ_STATUS_REG	0 /* Read only, returns status */
#define	SEQ_SLR_REG	1 /* Sequence Length Register (RW) */
#define	SEQ_OMR_ADD	2 /* Output Mask Register (RW) */
#define	SEQ_ADD		4 /* Sets the SEQ Mem Address Pointer */
#define	SEQ_DAT		5 /* Write or Read SEQ Memory */
#define	SEQ_CMD_DAT	6 /* Write to CMD FIFO (Write only) */


/* Set these to be outputs in the processor */

#define SEQ_INPUTS	(SEQ_ADDR|SEQ_RNW|SEQ_STR|SEQ_RESET)

/* And the other direction */

#define SEQ_OUTPUTS	(SEQ_ACK|SEQ_CMD_FIFO_FULL)

/* Assign these to the PIO */

#define SEQ_BITS	(SEQ_INPUTS|SEQ_DATA|SEQ_OUTPUTS)

/*
 *$Log$
 *Revision 1.1  2010-06-07 00:39:01  jpd
 *Massive reorganization of source tree.
 *
 *Revision 1.1  2009-03-26 01:45:01  jpd
 *First draft of sequencer driver.
 *
 */
