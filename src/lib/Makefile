# $Id$

AS = as
CC = gcc
AR = ar

%.o : %.S
	$(AS) -o $@ $<

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

OBJS =  init_lse.o strings.o lse-arm.o primitives.o

lse-arm.a : $(OBJS)
	$(AR) acs $(OBJS)

strings.o lse-arm.o primitives.o : lse-arm.h Makefile

init_lse.o : init.lse

# $Log$
# Revision 1.1  2009-03-10 20:37:11  jpd
# Makefile, ports.
#



