#
# Makefile for CSF Assignment 3
#

C_SRCS = //need to put something here
CFLAGS = -g -Og -no-pie -Wall -std=gnu11

LDFLAGS = -no-pie



solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp

clean :
	rm -f *.o solution.zip