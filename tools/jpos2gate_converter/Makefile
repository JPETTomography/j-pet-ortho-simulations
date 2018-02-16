CXX=g++
CXXFLAGS= -std=c++11 -Wall `root-config --cflags`
LDFLAGS= `root-config --ldflags --glibs`

OBJDIR=./obj
SRCDIR=src
H_FILES := $(wildcard $(SRCDIR)/*.h) 
CPP_FILES := $(wildcard $(SRCDIR)/*.cpp) $(SRCDIR)/MyGateOutputDict.cpp $(SRCDIR)/event.cpp 
OBJ_FILES := $(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

EVPATH = "$(shell pwd)/$(SRCDIR)/"
#checks if a dictionary exists
DICT_EXISTS=$(shell [ -e "$(shell pwd)/$(OBJDIR)/MyGateOutputDict.o" ] && echo 1 || echo 0 )

EVENT_FILES_PRESENT = 0
COPY_FILES = $(SRCDIR)/EventDict.cpp $(SRCDIR)/event.h $(SRCDIR)/event.cpp $(SRCDIR)/event_linkdef.h $(SRCDIR)/constants.h 

all:  converter 
	@echo "COMPILATION COMPLETE!!!"
	
converter: $(OBJDIR)/MyGateOutputDict.o $(OBJ_FILES) $(OBJDIR)/EventDict.o 
	@echo "Creating executable: $@"
	@echo ${CPP_FILES}
	@(cp $(SRCDIR)/*.pcm . &&  $(CXX) -o converter $^ $(LDFLAGS))

$(SRCDIR)/MyGateOutputDict.cpp: $(SRCDIR)/MyGateOutput.h $(COPY_FILES)
	@echo "Compiling $@"
	@(cd src && rootcint -f MyGateOutputDict.cpp -c $(CXXFLAGS) -p  MyGateOutput.h mygateoutput_linkdef.h)

$(OBJDIR)/MyGateOutputDict.o: $(SRCDIR)/MyGateOutputDict.cpp
	@echo "Compiling $@"
	@$(CXX) $(SRCDIR)/MyGateOutputDict.cpp -o $(OBJDIR)/MyGateOutputDict.o -c $(CXXFLAGS)
	
$(OBJDIR)/EventDict.o: ../../$(COPY_FILES)
	@echo "Compiling EventDict.o"
	@(cd src && rootcint -f EventDict.cpp -c $(CXXFLAGS) -p  event.h event_linkdef.h)
	@$(CC) $(SRCDIR)/EventDict.cpp -o $(OBJDIR)/EventDict.o -c $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.C
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(COPY_FILES):
	@echo "Copying file: $@"
	@cp ../../$@ ./src/

clean:
	@echo "Cleaning..."
	@rm -f $(SRCDIR)/*.gch $(SRCDIR)/*.d $(SRCDIR)/MyGateOutputDict.cpp $(SRCDIR)/*.so $(SRCDIR)/Auto* $(OBJDIR)/*.o $(SRCDIR)/MyGateOutputDict* MyGateOutputDict* converter
	@rm $(COPY_FILES)
	@rm *.pcm $(SRCDIR)/*.pcm



	