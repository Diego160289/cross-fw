PROJECT := FWLoader

LIBS += TinyXml

ifeq ($(OS), Nix)
    SYSLIBS += dl
else
    SYSLIBS +=
endif

include ./Make/make_common
