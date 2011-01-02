PROJECT := DBClient

ifeq ($(OS),Nix)
	SYSLIBS += pthread odbc
else
	SYSLIBS += ws2_32 odbc32
endif

include ./Make/make_common
