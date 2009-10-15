PROJECT := TestService2

ifeq ($(OS),Nix)
	SYSLIBS += pthread rt
else
	SYSLIBS +=
endif

include ./Make/make_common
