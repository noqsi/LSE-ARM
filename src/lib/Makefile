# $Id$

AS = as
CC = gcc
AR = ar
CFLAGS = -g -Wall

%.o : %.S
	$(AS) -o $@ $<

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

OBJS =  init_lse.o strings.o lse-arm.o primitives.o

liblse-arm.a : $(OBJS)
	$(AR) rcs liblse-arm.a $(OBJS)

strings.o lse-arm.o primitives.o : lse-arm.h Makefile

init_lse.txt : init.lse Makefile
	sed -e '/^#/d' -e '/^$$/d' init.lse >init_lse.txt

init_lse.o : init_lse.txt Makefile

test : testmain.o liblse-arm.a
	$(CC)  testmain.o -o test -L. -llse-arm -lm

# $Log$
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



