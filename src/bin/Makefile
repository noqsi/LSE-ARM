# $Id$

CFLAGS  = -g -O -I. -I../lib -mcpu=arm7tdmi -Wall -DCOPY_STATIC=1

ASFLAGS = -mcpu=arm7tdmi -gstabs
LDFLAGS = -Tsam7x256-exec-flash.ld -nostartfiles -Lgcc -L../lib 
LIBS = -llse-arm -lm

%.o:%.c
	arm-elf-gcc -c $(CFLAGS) $< -o $@

%.o:%.s
	arm-elf-as $(ASFLAGS) $< -o $@

LSE = boot.o C_startup.o lse.o usart.o

HDR = memory.h  peripheral_id.h  pio.h  pmc.h  usart.h  wdt.h usart_driver.h

all : lse.elf TESScamera.elf

$(LSE): $(HDR) Makefile

lse.elf: $(LSE) sam7x256-exec-flash.ld
	arm-elf-gcc $(LDFLAGS) -o $@ $(LSE) $(LIBS)
	
TESS = boot.o C_startup.o TESScamera.o TESSusart.o lse_seq.o sequencer.o
TESSHDR = $(HDR) usart.h usart_driver.h sequencer.h sequencer_bits.h

$(TESS): $(TESSHDR) Makefile

TESScamera.elf: $(TESS) sam7x256-exec-flash.ld
	arm-elf-gcc $(LDFLAGS) -o $@ $(TESS) $(LIBS)

clean:
	rm -f *.o *.elf

# $Log$
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
