include ./Make/make_common

PROJECT := TinyXml

SRCDIR = ./Sources/$(PROJECT)

OBJDIR = ./Objects/$(MAKETYPE)/$(PROJECT)

TARGETDIR = ./Libs/$(MAKETYPE)

INCLUDE = ./Include/Xml

TARGET = lib$(PROJECT).a

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
	ar cr \
	$(TARGETDIR)/$(TARGET) \
	$(addprefix $(OBJDIR)/,$(notdir $(OBJ)))
	@echo Success $(CURCFG)
