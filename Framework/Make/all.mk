all: make_bin

make_bin: make_libs make_dlls
	make -f ./Make/fwloader.mk

make_libs:
	make -f ./Make/common.mk
	make -f ./Make/system.mk

make_dlls: 
	make  -f ./Make/registry.mk
