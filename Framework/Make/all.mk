all: make_bin

make_bin: make_libs
	make -f ./Make/fwloader.mk

make_libs:
	make -f ./Make/common.mk
	make -f ./Make/system.mk
