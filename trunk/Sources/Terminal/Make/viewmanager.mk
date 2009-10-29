PROJECT := ViewManager

include ./Make/terminal_common

ifeq ($(OS),Nix)
	SYSLIBSDIR += /usr/X11R6/lib
	SYSLIBS += pthread X11
else
	SYSLIBS += Gdi32
endif

include ../Framework/Make/make_common
