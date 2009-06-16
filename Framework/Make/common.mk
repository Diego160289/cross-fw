include ./Make/make_common

SRCDIR = ./Sources/Common

OBJDIR = ./Objects/$(MAKETYPEDIR)/Common

TARGETDIR = ./Libs/$(MAKETYPEDIR)

INCLUDE = ./Include

TARGET = libCommon.a

CPP = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(CPP:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(addprefix -I ,$(INCLUDE)) -c $^ -o $(OBJDIR)/$(notdir $@)

common_debug: $(OBJ)
	ar cr $(TARGETDIR)/$(TARGET) $(addprefix $(OBJDIR)/,$(notdir $(OBJ)))
