PROJECT := FWLoader

LIBS += TinyXml

ifeq ($(OS), Nix)
    SYSLIBS += dl pthread
else
    SYSLIBS +=
endif

include ./Make/make_common
