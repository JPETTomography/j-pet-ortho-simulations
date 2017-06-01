CC=g++
CFLAGS=-c -Wall `root-config --cflags`
LDFLAGS=`root-config --ldflags --glibs`

all: sim

sim: simulate.o psdecay.o
	$(CC) -o sim simulate.o psdecay.o $(LDFLAGS)

simulate.o: simulate.cpp psdecay.cpp psdecay.h
	$(CC) $(CFLAGS) simulate.cpp psdecay.cpp psdecay.h 

psdecay.o: psdecay.cpp psdecay.h
	$(CC) $(CFLAGS) psdecay.cpp psdecay.h

clean: 
	rm *o sim
