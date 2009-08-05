PROJECT := ViewManager

include ./Make/extensions_common

ifeq ($(OS),Nix)
	SYSLIBS += pthread
else
	SYSLIBS += Gdi32
endif

include ../Framework/Make/make_common