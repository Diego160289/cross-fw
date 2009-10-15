PROJECT := Tools

ifeq ($(OS),Nix)
	SYSLIBS += pthread
else
	SYSLIBS +=
endif

include ./Make/make_common
