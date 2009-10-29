PROJECT := FlashView

include ./Make/extensions_common

LIBSDEP += TinyXml

ifeq ($(OS),Nix)
	SYSLIBS += pthread
else
	SYSLIBS += 
endif

include ../Framework/Make/make_common
