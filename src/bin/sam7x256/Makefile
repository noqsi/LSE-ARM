# $Id$

AS = arm-elf-as
CC = arm-elf-gcc

CFLAGS  = -O -I../../include -Wall

LIBDIR = ../../lib
BOOT= $(LIBDIR)/boot.o
LDFILE = sam7x256-exec-flash.ld
LDFLAGS = -nostartfiles -Lgcc -L$(LIBDIR) 
LIBS = -llse-arm -lm

# %.o:%.c
#	arm-elf-gcc -c $(CFLAGS) $< -o $@

# %.o:%.s
#	arm-elf-as $(ASFLAGS) $< -o $@

all : readout.elf

readout.o : readout.c Makefile

app_lse.o : readout_lse.txt

readout_lse.txt : readout.lse Makefile
	sed -e '/^#/d' -e '/^$$/d' readout.lse >readout_lse.txt

readout.elf:  $(LDFILE) readout.o app_lse.o
	arm-elf-gcc -T$(LDFILE) $(LDFLAGS) -o $@ $(BOOT) readout.o app_lse.o $(LIBS)

clean:
	rm -f *.o *.elf

# $Log$
# Revision 1.2  2010-07-15 17:52:21  jpd
# *** empty log message ***
#
# Revision 1.1  2010-06-07 00:39:01  jpd
# Massive reorganization of source tree.
#
# Revision 1.6  2010-06-05 17:32:44  jpd
# More reorganization.
#
# Revision 1.5  2010-06-04 18:13:17  jpd
# Update build for multiple targets.
#
# Revision 1.4  2009-03-26 02:10:13  jpd
# Can now compile seq stuff.
#
# Revision 1.3  2009-03-26 01:50:16  jpd
# Add seq driver.
#
# Revision 1.2  2009-03-23 02:42:04  jpd
# Version for TESS hardware.
#
# Revision 1.1  2009-03-14 22:58:06  jpd
# Can now run LSE in an ARM SAM7X!
#
