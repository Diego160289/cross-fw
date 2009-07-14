PROJECT := Tools

# LIBS = TinyXml

ifeq ($(OS), Win)
	LINKCXXFLAGS = 
else
	LINKCXXFLAGS = -lpthreads
endif

include ./Make/make_component
