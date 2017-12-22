CXX = g++
CXXFLAGS = -c -std=c++11 -Wall `root-config --cflags` #-DBOOST_NO_CXX11_SCOPED_ENUMS
LDFLAGS = -lgtest -lboost_filesystem -lboost_system -lpthread `root-config --ldflags --glibs` -lstdc++ -lTree
OBJDIR = ./obj
OBJDIRUP = ../obj
SRCDIR = src
SRCDIRUP = ../src


H_FILES := $(wildcard $(SRCDIRUP)/*.h)
CPP_FILES := $(wildcard $(SRCDIRUP)/*.cpp) 
CPP := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(CPP:.cpp=.o))) 
OBJS_FILES := $(OBJDIRUP)/psdecay.o $(OBJDIRUP)/initialcuts.o $(OBJDIRUP)/comptonscattering.o $(OBJDIRUP)/event.o $(OBJDIRUP)/parammanager.o $(OBJDIRUP)/EventDict.o  
INCS := $(H_FILES) $(CPP_FILES)
EVPATH = "$(shell pwd)/src/"

all: testAll

testAll: $(OBJS) $(OBJS_FILES)
	(cp $(SRCDIRUP)/EventDict_rdict.pcm . && $(CXX) -o testAll $(OBJS) $(OBJS_FILES) $(LDFLAGS))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm obj/*.o  testAll EventDict* 
