/* $Id */

/*
	Linker script for ARM SAM7X256

This script is intended for C programs that will reside in flash. Old fashioned
C layout of text, data, and bss bult up from the bottom, stack down from the
top, free memory in between. 

*/

/* The entry point will be named "boot" */

ENTRY(boot)

/* The following assumes bootstrap layout, no remap */


MEMORY
{
	flash	: org = 0, len = 0x40000
	sram	: org = 0x00200000, len = 0x10000
}

/* The stack occupies the highest RAM */

_stack_top = ORIGIN(sram) + LENGTH(sram);

SECTIONS
{
	.vectors :
	{
		*(.vectors)	/* interrupt and exception vectors */
	} >flash

	.glue :
	{
		*(.glue*);	/* The mysterious "glue" sections */
	} >flash
	
	.text : 		/* Code and read-only data */
	{
        	*(.text);	/* Machine code */
		*(.rodata*);	/* Read-only data */
	} >flash
	
	/* Initialized data: assigned to SRAM but loaded to flash */
	
	.data : AT( ADDR( .text ) + SIZEOF( .text ))
	{
		data_load_ = ADDR( .text ) + SIZEOF( .text );
		data_start_ = .;		
		*(.data);	/* Initialized data */
		data_end_ = .;
	} > sram
	
	.bss :				/* C_startup will zero this */
	{
               bss_start_ = . ;	/* Start zero fill here */
                *(.bss); *(COMMON)	/* Zeroed static data */
                bss_end_ = . ;	/* End zero fill here */
		end = .;		/* Start of free memory for libc */
	} > sram
}

/*
 * $Log$
 * Revision 1.1  2010-06-07 00:39:01  jpd
 * Massive reorganization of source tree.
 *
 * Revision 1.1  2009-03-14 22:58:06  jpd
 * Can now run LSE in an ARM SAM7X!
 *
 */
