CC=g++
CXXFLAGS= -std=c++11 -Wall `root-config --cflags`
LDFLAGS= `root-config --ldflags --glibs`

OBJDIR=./obj
SRCDIR=src
H_FILES := $(wildcard $(SRCDIR)/*.h) 
CPP_FILES := $(wildcard $(SRCDIR)/*.cpp) $(SRCDIR)/EventDict.cpp
OBJ_FILES := $(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

EVPATH = "$(shell pwd)/$(SRCDIR)/"
#checks if a dictionary exists
DICT_EXISTS=$(shell [ -e "$(shell pwd)/$(OBJDIR)/EventDict.o" ] && echo 1 || echo 0 )
	
all: sim
	@echo "COMPILATION COMPLETE!!!"

sim: $(OBJ_FILES) $(OBJDIR)/EventDict.o
	@echo "Creating executable: $@"
	@(cp $(SRCDIR)/*.pcm . &&  $(CC) -o sim $^ $(LDFLAGS))

$(SRCDIR)/EventDict.cpp: $(SRCDIR)/event.*
	@echo "Compiling $@"
	@(cd src && rootcint -f EventDict.cpp -c $(CXXFLAGS) -p  event.h event_linkdef.h)

$(OBJDIR)/EventDict.o: $(SRCDIR)/EventDict.cpp
	@echo "Compiling $@"
	@$(CC) $(SRCDIR)/EventDict.cpp -o $(OBJDIR)/EventDict.o -c $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $@"
	@$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning..."
	@rm -f $(SRCDIR)/*.gch $(SRCDIR)/*.d $(SRCDIR)/EventDict.cpp $(SRCDIR)/*.so $(SRCDIR)/Auto* $(OBJDIR)/*.o $(SRCDIR)/EventDict* EventDict* sim 
