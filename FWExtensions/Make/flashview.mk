PROJECT := FlashView

include ./Make/extensions_common

LIBS += TinyXml

ifeq ($(OS),Nix)
	SYSLIBS += pthread
else
	SYSLIBS += 
endif

include ../Framework/Make/make_common
