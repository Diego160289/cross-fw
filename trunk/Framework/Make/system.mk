PROJECT := System

ifeq ($(OS), Nix)
    INCLUDE += ./Include/Nix
endif

include ./Make/make_common
