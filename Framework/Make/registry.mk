include ./Make/make_common

PROJECT := Registry

SRCDIR = ./Sources/$(PROJECT)

OBJDIR = ./Objects/$(MAKETYPE)/$(PROJECT)

TARGETDIR = ./Bin/$(MAKETYPE)

INCLUDE = ./Include

TARGET = $(PROJECT)$(DLLFILEEXT)

CPP = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(CPP:.cpp=.o)

CURCFG := Build: '$(PROJECT)' $(OS) $(MAKETYPE)

%.o: %.cpp
	$(CXX) \
	$(CXXFLAGS) \
	$(addprefix -I ,$(INCLUDE)) \
	-c $^ \
	-o $(OBJDIR)/$(notdir $@)

all: $(OBJ)
	$(CXX) \
	$(CXXFLAGS) -shared \
	-o $(TARGETDIR)/$(TARGET) \
	$(addprefix $(OBJDIR)/,$(notdir $(OBJ)))
	@echo Success $(CURCFG)
