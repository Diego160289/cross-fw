PROJECT := Tools

# LIBS = TinyXml

ifeq ($(OS), Win)
	LINKCXXFLAGS = 
else
	LINKCXXFLAGS = -pthreads
endif

include ./Make/make_component
