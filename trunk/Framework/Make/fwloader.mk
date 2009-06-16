include ./Make/make_common

PROJECT := FWLoader

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

%.o: %.cpp
	$(CXX) \
	$(CXXFLAGS) \
	$(addprefix -I ,$(INCLUDE)) \
	-c $^ \
	-o $(OBJDIR)/$(notdir $@)

all: $(OBJ)
	@echo $(CURCFG)
	$(CXX) \
	$(addprefix $(OBJDIR)/,$(notdir $(OBJ))) \
	$(addprefix -L ,$(LIBSDIR)) \
	$(addprefix -l ,$(LIBS)) \
	-o $(TARGETDIR)/$(TARGET)
	@echo Success $(CURCFG)
