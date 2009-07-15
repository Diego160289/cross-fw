#ll: make_bin

make_bin: make_libs make_dlls make_tools
	make -f ./Make/fwloader.mk

make_libs:
	make TARGETTYPE=Lib -f ./Make/common.mk
	make TARGETTYPE=Lib -f ./Make/system.mk
	make TARGETTYPE=Lib -f ./Make/tinyxml.mk

make_dlls: 
	make TARGETTYPE=Dll -f ./Make/registry.mk
	make TARGETTYPE=Dll -f ./Make/classfactory.mk
	make TARGETTYPE=Dll -f ./Make/tools.mk

make_tools:
#	make  -f ./Make/cmpregclient.mk
