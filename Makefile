CC=g++
CFLAGS=-c -std=c++11 -Wall `root-config --cflags`
LDFLAGS=`root-config --ldflags --glibs`

all: sim

sim: simulate.o psdecay.o comptonscattering.o parammanager.o
	$(CC) -o sim simulate.o psdecay.o comptonscattering.o parammanager.o $(LDFLAGS)

psdecay.o: psdecay.cpp psdecay.h comptonscattering.h constants.h
	$(CC) $(CFLAGS) psdecay.cpp psdecay.h #comptonscattering.h constants.h

comptonscattering.o: comptonscattering.cpp comptonscattering.h constants.h
	$(CC) $(CFLAGS) comptonscattering.cpp comptonscattering.h constants.h

parammanager.o: parammanager.cpp parammanager.h
	$(CC) $(CFLAGS) parammanager.cpp parammanager.h

simulate.o: simulate.cpp psdecay.h comptonscattering.h constants.h parammanager.h
	$(CC) $(CFLAGS) simulate.cpp #psdecay.h comptonscattering.h constants.h

clean: 
	rm *.gch *.o sim
