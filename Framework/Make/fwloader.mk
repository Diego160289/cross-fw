PROJECT := FWLoader

ifeq ($(OS), Nix)
    SYSLIBS += dl
else
    SYSLIBS +=
endif

include ./Make/make_common
