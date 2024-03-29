FWDIR := ./

all: create_md_command make_bin	clean

make_bin: make_libs make_dlls make_tools
	make TARGETTYPE=Exe -f ./Make/fwloader.mk

make_libs:
	make TARGETTYPE=Lib -f ./Make/common.mk
	make TARGETTYPE=Lib -f ./Make/system.mk
	make TARGETTYPE=Lib -f ./Make/tinyxml.mk
	make TARGETTYPE=Lib -f ./Make/registry.mk

make_dlls: 
	make TARGETTYPE=Dll -f ./Make/registry.mk
	make TARGETTYPE=Dll -f ./Make/classfactory.mk
	make TARGETTYPE=Dll -f ./Make/servicemanager.mk
	make TARGETTYPE=Dll -f ./Make/network.mk
	make TARGETTYPE=Dll -f ./Make/dbclient.mk
	make TARGETTYPE=Dll -f ./Make/logger.mk

make_tools:
	make TARGETTYPE=Exe -f ./Make/cmpregclient.mk

ifeq ($(OS), Win)
create_md_command: ./Sources/MyWinMkDir/MyWinMkDir.cpp
	g++ -O3 -Wall \
	$(FWDIR)Sources/MyWinMkDir/MyWinMkDir.cpp \
	-o $(FWDIR)Sources/MyWinMkDir/MyWinMkDir.exe

else
create_md_command:

endif

ifeq ($(OS), Win)
clean:
	rd /S /Q .\Objects

else
clean:
	rm -r -f ./Objects

endif
