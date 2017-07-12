CC=g++
CFLAGS=-c -std=c++11 -Wall `root-config --cflags`
LDFLAGS=`root-config --ldflags --glibs` -lTree

OBJDIR=./obj
SRCDIR=src
EVPATH = "$(shell pwd)/$(SRCDIR)/"

all: sim

sim: $(OBJDIR)/simulate.o $(OBJDIR)/event.o $(OBJDIR)/psdecay.o $(OBJDIR)/initialcuts.o $(OBJDIR)/comptonscattering.o $(OBJDIR)/parammanager.o
	$(CC) -o sim $(OBJDIR)/simulate.o $(OBJDIR)/event.o $(OBJDIR)/psdecay.o $(OBJDIR)/initialcuts.o $(OBJDIR)/comptonscattering.o $(OBJDIR)/parammanager.o $(SRCDIR)/event_cpp.so $(LDFLAGS)	
	
$(OBJDIR)/event.o: $(SRCDIR)/event.cpp $(SRCDIR)/event.h
	root -l -b -n -q $(SRCDIR)/'generatedict.cpp($(EVPATH))'++
	$(CC) $(CFLAGS) $(SRCDIR)/event.cpp -o $(OBJDIR)/event.o

$(OBJDIR)/psdecay.o: $(SRCDIR)/psdecay.cpp $(SRCDIR)/psdecay.h $(SRCDIR)/comptonscattering.h $(SRCDIR)/constants.h
	$(CC) $(CFLAGS) $(SRCDIR)/psdecay.cpp -o $(OBJDIR)/psdecay.o

$(OBJDIR)/initialcuts.o: $(SRCDIR)/initialcuts.cpp $(SRCDIR)/initialcuts.h $(SRCDIR)/event.h
	$(CC) $(CFLAGS) $(SRCDIR)/initialcuts.cpp -o $(OBJDIR)/initialcuts.o

$(OBJDIR)/comptonscattering.o: $(SRCDIR)/comptonscattering.cpp $(SRCDIR)/comptonscattering.h $(SRCDIR)/constants.h
	$(CC) $(CFLAGS) $(SRCDIR)/comptonscattering.cpp -o $(OBJDIR)/comptonscattering.o

$(OBJDIR)/parammanager.o: $(SRCDIR)/parammanager.cpp $(SRCDIR)/parammanager.h
	$(CC) $(CFLAGS) $(SRCDIR)/parammanager.cpp -o $(OBJDIR)/parammanager.o

$(OBJDIR)/simulate.o: $(SRCDIR)/simulate.cpp $(SRCDIR)/psdecay.h $(SRCDIR)/comptonscattering.h $(SRCDIR)/constants.h $(SRCDIR)/parammanager.h
	#root -l -b -n -q $(SRCDIR)/simulate.cpp++
	$(CC) $(CFLAGS) $(SRCDIR)/simulate.cpp -o $(OBJDIR)/simulate.o

clean: 
	rm $(SRCDIR)/*.gch $(SRCDIR)/*.d $(SRCDIR)/*.so $(OBJDIR)/*.o sim

