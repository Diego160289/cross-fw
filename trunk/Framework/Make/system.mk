include ./Make/make_common

PROJECT := System

SRCDIR = ./Sources/$(PROJECT)

OSSRCDIR = ./Sources/$(PROJECT)/$(OS)

OBJDIR = ./Objects/$(MAKETYPE)/$(PROJECT)

TARGETDIR = ./Libs/$(MAKETYPE)

INCLUDE = ./Include $(SRCDIR)/$(OS)

TARGET = lib$(PROJECT).a

CPP = $(wildcard $(SRCDIR)/*.cpp)

CPP += $(wildcard $(OSSRCDIR)/*.cpp)

OBJ = $(CPP:.cpp=.o)

CURCFG := Build: '$(PROJECT)' $(OS) $(MAKETYPE)

%.o: %.cpp
	$(CXX) \
	$(CXXFLAGS) \
	$(addprefix -I ,$(INCLUDE)) \
	-c $^ \
	-o $(OBJDIR)/$(subst /$(OS)/,Platform/,$(findstring /$(OS)/,$^))$(notdir $@)

all: $(OBJ)
	@echo $(CURCFG)
	ar cr \
	$(TARGETDIR)/$(TARGET) \
	$(wildcard $(OBJDIR)/Platform/*.o) \
	$(wildcard $(OBJDIR)/*.o)
	@echo Success $(CURCFG)
