FWDIR = ../Framework/

all: make_bin	clean

make_bin: make_libs make_dlls make_tools
#	make TARGETTYPE=Exe -f ./Make/fwloader.mk

make_libs:
#	make TARGETTYPE=Lib FWDIR=../Framework/ -C ./ -f ./Make/.mk

make_dlls: 
	make TARGETTYPE=Dll FWDIR=../Framework/ -C ./ -f ./Make/viewmanager.mk
	make TARGETTYPE=Dll FWDIR=../Framework/ -C ./ -f ./Make/flashview.mk

make_tools:
#	make TARGETTYPE=Exe FWDIR=../Framework/ -C ./ -f ./Make/.mk

ifeq ($(OS), Win)
clean:
	rd /S /Q .\Objects

else
clean:
	rm -r -f ./Objects

endif
