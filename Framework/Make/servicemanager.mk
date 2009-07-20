PROJECT := ServiceManager

ifeq ($(OS),Nix)
	SYSLIBS += pthread rt
	
	LIBS += UuidLib
else
	SYSLIBS +=
endif

include ./Make/make_common
