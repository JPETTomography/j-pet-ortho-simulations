DEP = ../src
CXX = g++
#-I$(DEP)
CXXFLAGS = -c -std=c++11 -Wall `root-config --cflags` #-DBOOST_NO_CXX11_SCOPED_ENUMS
LDFLAGS = -lgtest -lboost_filesystem -lboost_system -lpthread `root-config --ldflags --glibs` -lstdc++ -lTree
OBJDIR = ./obj
OBJDIRUP = ../obj
SRCDIR = src


INCS = ./$(SRCDIR)/main_test_all.cpp ./$(SRCDIR)/cuts_tests.cpp ./$(SRCDIR)/event.h $(SRCDIR)/initialcuts.h $(SRCDIR)/parammanager.h

OBJS =  $(OBJDIR)/main_test_all.o $(OBJDIR)/event.o $(OBJDIR)/cuts_tests.o  $(OBJDIR)/initialcuts.o $(OBJDIR)/parammanager.o
EVPATH = "$(shell pwd)/src/"


#../obj/parammanager.o ../obj/psdecay.o ../obj/comptonscattering.o

testAll: $(OBJS) $(INCS)
	$(CXX) -o testAll $(OBJS) $(LDFLAGS)
	
$(OBJDIR)/main_test_all.o: $(INCS)
	$(CXX) $(CXXFLAGS) ./$(SRCDIR)/main_test_all.cpp -o $(OBJDIR)/main_test_all.o 

$(OBJDIR)/cuts_tests.o: $(INCS) 
	$(CXX) $(CXXFLAGS) ./$(SRCDIR)/cuts_tests.cpp  -o $(OBJDIR)/cuts_tests.o 

$(OBJDIR)/event.o: $(INCS)
	cp $(DEP)/event.h $(DEP)/event.cpp $(DEP)/generatedict.cpp $(DEP)/event_linkdef.h ./$(SRCDIR)
	root -l -b -n -q $(SRCDIR)/'generatedict.cpp($(EVPATH))'++
	$(CC) $(CXXFLAGS) $(SRCDIR)/event.cpp -o $(OBJDIR)/event.o

$(OBJDIR)/initialcuts.o: $(SRCDIR)/initialcuts.cpp $(SRCDIR)/initialcuts.h $(SRCDIR)/event.h
	$(CC) $(CXXFLAGS) $(SRCDIR)/initialcuts.cpp -o $(OBJDIR)/initialcuts.o

$(OBJDIR)/parammanager.o: $(SRCDIR)/parammanager.cpp $(SRCDIR)/parammanager.h
	$(CC) $(CXXFLAGS) $(SRCDIR)/parammanager.cpp -o $(OBJDIR)/parammanager.o	
	
clean:
	rm obj/*.o $(SRCDIR)/*.so $(SRCDIR)/*.d testAll  testAll.xml 
