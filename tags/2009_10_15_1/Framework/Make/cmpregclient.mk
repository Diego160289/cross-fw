PROJECT := CmpRegClient

LIBS += Registry TinyXml

ifeq ($(OS),Nix)
	SYSLIBS += pthread dl
else
	SYSLIBS +=
endif

include ./Make/make_common
