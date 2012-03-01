# $Id$

CFLAGS  = -g -O -I../../include -mcpu=arm7tdmi -msoft-float -Wall

LIBDIR = ../../lib
BOOT= $(LIBDIR)/boot.o
LDFILE = sam7a3-exec-flash.ld
ASFLAGS = -mcpu=arm7tdmi -msoft-float -gstabs
LDFLAGS = -msoft-float -nostartfiles -Lgcc -L$(LIBDIR) 
LIBS = -llse-arm -lm
ELF_FILE = lse-mpu.elf

%.o:%.c
	arm-elf-gcc -c $(CFLAGS) $< -o $@

%.o:%.s
	arm-elf-as $(ASFLAGS) $< -o $@

	
lse-mpu.elf:  $(LDFILE) lse-mpu.o mpu-tc.o
	arm-elf-gcc -T$(LDFILE) $(LDFLAGS) -o $@ \
		$(BOOT) lse-mpu.o mpu-tc.o $(LIBS)

upload: lse-mpu.elf /tmp/LCK..openocd
	echo "reset; halt; flash write_image lse-mpu.elf 0x100000; reset;" \
	 | telnet localhost 4444 \
	|| sleep 15

/tmp/LCK..openocd:
	touch /tmp/LCK..openocd
	(openocd 2>openocd.log; rm /tmp/LCK..openocd) & sleep 2

clean:
	rm -f *.o *.elf openocd.log

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
