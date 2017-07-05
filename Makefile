CC=g++
CFLAGS=-c -std=c++11 -Wall `root-config --cflags`
LDFLAGS=`root-config --ldflags --glibs`

OBJDIR=./obj
SRCDIR=src

all: sim

sim: $(OBJDIR)/simulate.o $(OBJDIR)/psdecay.o $(OBJDIR)/comptonscattering.o $(OBJDIR)/parammanager.o
	$(CC) -o sim $(OBJDIR)/simulate.o $(OBJDIR)/psdecay.o $(OBJDIR)/comptonscattering.o $(OBJDIR)/parammanager.o $(LDFLAGS)

$(OBJDIR)/psdecay.o: $(SRCDIR)/psdecay.cpp $(SRCDIR)/psdecay.h $(SRCDIR)/comptonscattering.h $(SRCDIR)/constants.h
	$(CC) $(CFLAGS) $(SRCDIR)/psdecay.cpp -o $(OBJDIR)/psdecay.o

$(OBJDIR)/comptonscattering.o: $(SRCDIR)/comptonscattering.cpp $(SRCDIR)/comptonscattering.h $(SRCDIR)/constants.h
	$(CC) $(CFLAGS) $(SRCDIR)/comptonscattering.cpp -o $(OBJDIR)/comptonscattering.o

$(OBJDIR)/parammanager.o: $(SRCDIR)/parammanager.cpp $(SRCDIR)/parammanager.h
	$(CC) $(CFLAGS) $(SRCDIR)/parammanager.cpp -o $(OBJDIR)/parammanager.o

$(OBJDIR)/simulate.o: $(SRCDIR)/simulate.cpp $(SRCDIR)/psdecay.h $(SRCDIR)/comptonscattering.h $(SRCDIR)/constants.h $(SRCDIR)/parammanager.h
	$(CC) $(CFLAGS) $(SRCDIR)/simulate.cpp -o $(OBJDIR)/simulate.o

clean: 
	rm $(SRCDIR)/*.gch $(OBJDIR)/*.o sim

