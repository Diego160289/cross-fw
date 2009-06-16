all_debug: make_libs make_bin

make_libs:
	make -f ./Make/common.mk
	make -f ./Make/system.mk

make_bin:
	make -f ./Make/fwloader.mk
