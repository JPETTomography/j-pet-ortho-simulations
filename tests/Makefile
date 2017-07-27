DEP = ../src
CXX = g++
#-I$(DEP)
CXXFLAGS = -c -std=c++11 -Wall `root-config --cflags` #-DBOOST_NO_CXX11_SCOPED_ENUMS
LDFLAGS = -lgtest -lboost_filesystem -lboost_system -lpthread `root-config --ldflags --glibs` -lstdc++ -lTree
OBJDIR = ./obj
OBJDIRUP = ../obj
SRCDIR = src
SRCDIRUP = ../src


INCS = ./$(SRCDIR)/main_test_all.cpp ./$(SRCDIR)/cuts_tests.cpp ./$(SRCDIRUP)/event.h 

OBJS =  $(OBJDIR)/main_test_all.o $(OBJDIRUP)/event.o $(OBJDIR)/cuts_tests.o  $(OBJDIRUP)/initialcuts.o $(OBJDIRUP)/parammanager.o $(OBJDIRUP)/EventDict.o
EVPATH = "$(shell pwd)/src/"


#../obj/parammanager.o ../obj/psdecay.o ../obj/comptonscattering.o

testAll: $(OBJS) $(INCS)
	(cp $(SRCDIRUP)/EventDict_rdict.pcm . && $(CXX) -o testAll $(OBJS) $(LDFLAGS))
	
$(OBJDIR)/main_test_all.o: $(INCS)
	$(CXX) $(CXXFLAGS) ./$(SRCDIR)/main_test_all.cpp -o $(OBJDIR)/main_test_all.o 

$(OBJDIR)/cuts_tests.o: $(INCS) 
	$(CXX) $(CXXFLAGS) ./$(SRCDIR)/cuts_tests.cpp  -o $(OBJDIR)/cuts_tests.o 

clean:
	rm obj/*.o $(SRCDIR)/*.so $(SRCDIR)/*.d testAll  testAll.xml 
