PROJECT := ControlCenter

include ./Make/terminal_common

INCLUDE += ./Include/OGLUI

ifeq ($(OS),Nix)
else
endif

include ../Framework/Make/make_common
