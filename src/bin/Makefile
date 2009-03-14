# $Id$

CFLAGS  = -g -O -I. -mcpu=arm7tdmi -Wall -DCOPY_STATIC=1

ASFLAGS = -mcpu=arm7tdmi -gstabs
LDFLAGS = -Tsam7x256-exec-flash.ld -nostartfiles -Lgcc -L../lib 
LIBS = -llse-arm -lm

%.o:%.c
	arm-elf-gcc -c $(CFLAGS) $< -o $@

%.o:%.s
	arm-elf-as $(ASFLAGS) $< -o $@

LSE = boot.o C_startup.o lse.o usart.o

HDR = memory.h  peripheral_id.h  pio.h  pmc.h  usart.h  wdt.h usart_driver.h

all : lse.elf

$(OBJS): $(HDR) Makefile

lse.elf: $(LSE) sam7x256-exec-flash.ld
	arm-elf-gcc $(LDFLAGS) -o $@ $(LSE) $(LIBS)

clean:
	rm -f *.o *.elf

# $Log$
# Revision 1.1  2009-03-14 22:58:06  jpd
# Can now run LSE in an ARM SAM7X!
#
