# $Id$

CFLAGS  = -g -O -I. -I../lib -mcpu=arm7tdmi -Wall -DCOPY_STATIC=1

LD7A3 = sam7a3-exec-flash.ld
LD7x256 = sam7x256-exec-flash.ld
ASFLAGS = -mcpu=arm7tdmi -gstabs
LDFLAGS = -nostartfiles -Lgcc -L../lib 
LIBS = -llse-arm -lm

%.o:%.c
	arm-elf-gcc -c $(CFLAGS) $< -o $@

%.o:%.s
	arm-elf-as $(ASFLAGS) $< -o $@

LSE = boot.o C_startup.o lse.o usart.o

HDR = memory_7A3.h  memory_7x256.h peripheral_id.h  pio.h  pmc.h \
      usart.h wdt.h usart_driver.h

all : lse7A3.elf lse7x256.elf TESScamera.elf

$(LSE): $(HDR) Makefile

lse7x256.elf: $(LSE) $(LD7x256)
	arm-elf-gcc -T$(LD7x256) -DLD7x256=1 $(LDFLAGS) -o $@ $(LSE) $(LIBS)

lse7A3.elf: $(LSE) $(LD7A3)
	arm-elf-gcc -T$(LD7A3) -DLD7A3=1 $(LDFLAGS) -o $@ $(LSE) $(LIBS)
	
TESS = boot.o C_startup.o TESScamera.o TESSusart.o lse_seq.o sequencer.o
TESSHDR = $(HDR) usart.h usart_driver.h sequencer.h sequencer_bits.h

$(TESS): $(TESSHDR) Makefile

TESScamera.elf: $(TESS) $(LD7x256)
	arm-elf-gcc -T$(LD7x256) -DLD7x256=1 $(LDFLAGS) -o $@ $(TESS) $(LIBS)

clean:
	rm -f *.o *.elf

# $Log$
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
