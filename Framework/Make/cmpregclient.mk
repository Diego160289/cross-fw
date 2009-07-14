include ./Make/make_common

PROJECT := CmpRegClient

SRCDIR = ./Sources/$(PROJECT)

OBJDIR = ./Objects/$(MAKETYPE)/$(PROJECT)

TARGETDIR = ./Bin/$(MAKETYPE)

INCLUDE = ./Include

LIBSDIR = ./Libs/$(MAKETYPE)

LIBS = Common System

TARGET = $(PROJECT)$(EXEFILEEXT)

CPP = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(CPP:.cpp=.o)

CURCFG := Build: '$(PROJECT)' $(OS) $(MAKETYPE)

ifeq ($(OS),Nix)
	SYSLIBS += dl
else
	SYSLIBS =
endif

%.o: %.cpp
	$(CXX) \
	$(CXXFLAGS) \
	$(addprefix -I ,$(INCLUDE)) \
	-c $^ \
	-o $(OBJDIR)/$(notdir $@)

all: $(OBJ)
	$(CXX) \
	$(addprefix $(OBJDIR)/,$(notdir $(OBJ))) \
	$(addprefix -L ,$(LIBSDIR)) \
	$(addprefix -l ,$(LIBS)) \
	$(addprefix -l ,$(SYSLIBS)) \
	-o $(TARGETDIR)/$(TARGET)
	@echo Success $(CURCFG)
