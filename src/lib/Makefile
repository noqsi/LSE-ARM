# $Id$

AS = arm-elf-as
CC = arm-elf-gcc
AR = arm-elf-ar
CFLAGS = -g -Wall -I../include

%.o : %.s
	$(AS) -o $@ $<

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

OBJS =  init_lse.o strings.o lse-arm.o primitives.o usart.o C_startup.o copy_static.o aic.o \
	irq_stack.o pit.o spi_driver.o

all : boot.o liblse-arm.a

liblse-arm.a : $(OBJS)
	$(AR) rcs liblse-arm.a $(OBJS)

$(OBJS) : Makefile

init_lse.txt : init.lse Makefile
	sed -e '/^#/d' -e '/^$$/d' init.lse >init_lse.txt

init_lse.o : init_lse.txt Makefile

test : testmain.o liblse-arm.a
	$(CC)  testmain.o -o test -L. -llse-arm -lm

clean : 
	rm -f *.a *.o init_lse.txt test

# $Log$
# Revision 1.9  2010-07-13 18:38:07  jpd
# First draft of low level SPI driver.
#
# Revision 1.8  2010-06-10 17:53:07  jpd
# Completed interrupt infrastructure.
# Periodic timer interrupt working on SAM7A3.
# Commented out some unnecessary definitions.
# Added ability to display free memory.
#
# Revision 1.7  2010-06-08 18:57:41  jpd
# Faults and user interrupts now work on SAM7A3
#
# Revision 1.6  2010-06-07 00:39:01  jpd
# Massive reorganization of source tree.
#
# Revision 1.5  2009-06-12 21:00:57  jpd
# Remove %.s : %.S rule.
# Add clean.
#
# Revision 1.4  2009-03-26 01:26:22  jpd
# Better factoring.
#
# Revision 1.3  2009-03-11 03:20:28  jpd
# This version runs!
#
# Revision 1.2  2009-03-11 02:19:42  jpd
# It compiles, executes.
# Prompt doesn't work.
# OS hooks need removal.
#
# Revision 1.1  2009-03-10 20:37:11  jpd
# Makefile, ports.
#



