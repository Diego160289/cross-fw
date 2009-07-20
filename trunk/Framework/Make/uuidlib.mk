PROJECT := UuidLib

ifeq ($(OS), Nix)
    INCLUDE += ./Include/Nix
endif

include ./Make/make_common
