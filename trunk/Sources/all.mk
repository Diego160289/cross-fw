all:
	make -C ./Framework -f ./Make/all.mk all
	make -C ./Terminal -f ./Make/all.mk all
	make -C ./Logic -f ./Make/all.mk all
