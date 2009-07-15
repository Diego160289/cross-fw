PROJECT := ClassFactory

ifeq ($(OS),Nix)
	SYSLIBS += pthread dl rt
else
	SYSLIBS +=
endif

include ./Make/make_common
