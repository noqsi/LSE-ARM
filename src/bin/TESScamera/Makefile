# $Id$

AS = arm-elf-as
CC = arm-elf-gcc

CFLAGS  = -g -O -I../../include -mcpu=arm7tdmi -msoft-float -Wall

LIBDIR = ../../lib
BOOT= $(LIBDIR)/boot.o
LDFILE = sam7x256-exec-flash.ld 
ASFLAGS = -mcpu=arm7tdmi -msoft-float -gstabs
LDFLAGS = -msoft-float -nostartfiles -Lgcc -L$(LIBDIR) 
LIBS = -llse-arm -lm

UPLOAD =	openocd -d0 --file=openocd.cfg \
		-c 'init;init;arm7_9 fast_memory_access enable' \
		-c 'halt;flash write_image $< 0x100000;reset' \
		-c 'shutdown'

# This rule strips comments and blank lines from LSE source.

%.txt:%.lse
	sed -e '/^#/d' -e '/^$$/d' $< >$@

TESSTUFF = lse_seq.o sequencer.o

# The version that runs on the EK board
# For now, use the c20 version of LSE code

upload_ek: TESSek.elf
	$(UPLOAD)

TESSek.elf:  $(LDFILE) $(TESSTUFF) TESSek.o app_lse_ek.o
	$(CC) -T$(LDFILE) $(LDFLAGS) -o $@ $(BOOT)  TESSek.o \
	$(TESSTUFF) $(LIBS) app_lse_ek.o

TESSek.o : TESScamera.c
	$(CC) $(CFLAGS) -c -DEKBOARD=1 -o $@ TESScamera.c

app_lse_ek.o : ek.txt

# The version for CCID-20 with TESS electronics

upload_c20 : TESSc20.elf
	$(UPLOAD)

TESSc20.elf:  $(LDFILE) $(TESSTUFF) TESScamera.o app_lse_c20.o
	$(CC) -T$(LDFILE) $(LDFLAGS) -o $@ $(BOOT) TESScamera.o \
	$(TESSTUFF) $(LIBS) app_lse_c20.o

app_lse_c20.o : c20.txt


clean:
	rm -f *.o *.elf

# $Log$
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
