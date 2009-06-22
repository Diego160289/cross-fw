include ./Make/make_common

PROJECT := Registry

SRCDIR = ./Sources/$(PROJECT)

OBJDIR = ./Objects/$(MAKETYPE)/$(PROJECT)

TARGETDIR = ./Bin/$(MAKETYPE)

INCLUDE = ./Include

LIBSDIR = ./Libs/$(MAKETYPE)

LIBS = Common System TinyXml

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
	$(addprefix $(OBJDIR)/,$(notdir $(OBJ))) \
	$(addprefix -L ,$(LIBSDIR)) \
	$(addprefix -l ,$(LIBS)) \
	-o $(TARGETDIR)/$(TARGET)
	@echo Success $(CURCFG)
