#
# Makefile for CSF Assignment 3
#

CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic 

LDFLAGS = -no-pie

%.o : %.c
	gcc $(CXXFLAGS) -cpp $<

csim : main

main : main.o
	gcc -o $@ $(LDFLAGS) main.o

solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp *.S

clean :
	rm -f *.o solution.zip