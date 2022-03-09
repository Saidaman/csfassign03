#
# Makefile for CSF Assignment 3
#

CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic 

LDFLAGS =

%.o : %.c
	$(CXX) $(CXXFLAGS) -cpp $<

csim : main.o #add others?
	$(CXX) -o $@ $(LDFLAGS) main.o

solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp *.S

clean :
	rm -f *.o csim solution.zip