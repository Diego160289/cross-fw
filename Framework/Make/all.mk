#ll: make_bin

make_bin: make_libs make_dlls make_tools
	make -f ./Make/fwloader.mk

make_libs:
	make -f ./Make/common.mk
	make -f ./Make/system.mk
	make -f ./Make/tinyxml.mk

make_dlls: 
	make  -f ./Make/registry.mk
	make  -f ./Make/classfactory.mk
	make  -f ./Make/tools.mk

make_tools:
	make  -f ./Make/cmpregclient.mk
