PROJECT := ControlCenter

include ./Make/terminal_common

INCLUDE += ./Include/OGLUI

LIBS += OGLUI

ifeq ($(OS),Nix)
else
endif

include ../Framework/Make/make_common
