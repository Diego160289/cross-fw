FWDIR = ../Framework/

all: make_bin	clean

make_bin: make_libs make_dlls make_tools
#	make TARGETTYPE=Exe -f ./Make/fwloader.mk

make_libs:
#	make TARGETTYPE=Lib FWDIR=Framework -f ./Make/common.mk

make_dlls: 
	make TARGETTYPE=Dll FWDIR=../Framework/ -C ./ -f ./Make/mainservice.mk

make_tools:
#	make TARGETTYPE=Exe -C ./ -f ./Make/cmpregclient.mk

ifeq ($(OS), Win)
clean:
	rd /S /Q .\Objects

else
clean:
	rm $(FWDIR)Objects

endif
