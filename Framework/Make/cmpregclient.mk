PROJECT := CmpRegClient

LIBS += TinyXml Registry

ifeq ($(OS),Nix)
	SYSLIBS += pthread dl
else
	SYSLIBS +=
endif

include ./Make/make_common
