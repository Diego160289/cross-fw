include ./Make/make_common

SRCDIR = ./Sources/System

OSSRCDIR = ./Sources/System/$(OS)

OBJDIR = ./Objects/$(MAKETYPE)/System

TARGETDIR = ./Libs/$(MAKETYPE)

INCLUDE = ./Include $(SRCDIR)/$(OS)

TARGET = libSystem.a

CPP = $(wildcard $(SRCDIR)/*.cpp)

CPP += $(wildcard $(OSSRCDIR)/*.cpp)

OBJ = $(CPP:.cpp=.o)

%.o: %.cpp
	$(CXX) \
	$(CXXFLAGS) \
	$(addprefix -I ,$(INCLUDE)) \
	-c $^ \
	-o $(OBJDIR)/$(subst /$(OS)/,Platform/,$(findstring /$(OS)/,$^))$(notdir $@)

all: $(OBJ)
	ar cr \
	$(TARGETDIR)/$(TARGET) \
	$(wildcard $(OBJDIR)/Platform/*.o) \
	$(wildcard $(OBJDIR)/*.o)
