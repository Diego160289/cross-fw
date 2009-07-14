PROJECT := ClassFactory

ifeq ($(OS),Nix)
	SYSLIBS += pthread dl
else
	SYSLIBS =
endif

include ./Make/make_component
