all:
	make -C ./Framework -f ./Make/all.mk all
	make -C ./FWExtensions -f ./Make/all.mk all
	make -C ./Terminal -f ./Make/all.mk all
