PROJECT := Network

ifeq ($(OS),Nix)
	SYSLIBS += pthread
else
	SYSLIBS += ws2_32
endif

include ./Make/make_common
