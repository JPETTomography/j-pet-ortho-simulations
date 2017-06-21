DEP = ../src
CXX = g++
CXXFLAGS = -c -std=c++11 -Wall `root-config --cflags` -I$(DEP)
LDFLAGS = -lgtest -lpthread `root-config --ldflags --glibs` -lstdc++
OBJDIR = ./obj
SRCDIR = ./src
INCS = ./src/main_test_all.cpp ./src/decay_tests.cpp
OBJS =  $(OBJDIR)/main_test_all.o $(OBJDIR)/decay_tests.o ../obj/psdecay.o
#../obj/parammanager.o ../obj/psdecay.o ../obj/comptonscattering.o

testAll: $(OBJS) $(DEP)/psdecay.h
	$(CXX) -o testAll $(OBJS) $(LDFLAGS)
	
$(OBJDIR)/main_test_all.o: $(INCS)
	$(CXX) $(CXXFLAGS) $(SRCDIR)/main_test_all.cpp -o $(OBJDIR)/main_test_all.o $(LDFLAGS)

$(OBJDIR)/decay_tests.o: $(INCS) 
	$(CXX) $(CXXFLAGS) $(SRCDIR)/decay_tests.cpp  -o $(OBJDIR)/decay_tests.o 

clean:
	rm testAll obj/*.o testAll.xml
