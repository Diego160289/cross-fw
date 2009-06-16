include ./Make/make_common

SRCDIR = ./Sources/FWLoader

OBJDIR = ./Objects/$(MAKETYPE)/FWLoader

TARGETDIR = ./Bin/$(MAKETYPE)

INCLUDE = ./Include

LIBSIR = ./Libs/$(MAKETYPE)

LIBS = Common System

TARGET = FWLoader.exe

CPP = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(CPP:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(addprefix -I ,$(INCLUDE)) -c $^ -o $(OBJDIR)/$(notdir $@)

all: $(OBJ)
	$(CXX) $(addprefix $(OBJDIR)/,$(notdir $(OBJ))) -L $(LIBSIR) $(addprefix -l ,$(LIBS)) -o $(TARGETDIR)/$(TARGET)
