PROJECT := ControlCenter

include ./Make/terminal_common

INCLUDE += ./Include/OGLUI

LIBS += OGLUI


ifeq ($(OS),Nix)
else
	LIBS += gdi32 opengl32 glu32
endif


include ../Framework/Make/make_common
