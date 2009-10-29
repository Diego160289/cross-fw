all: compile
	gcc np_host.o system.o gtk2xtbin.o flash.o flash_dll.o Test.o -o Test \
	    `pkg-config --libs gtk+-2.0` -lXaw -lXmu -lXt -lSM -lICE -lXpm -lXext -lX11

compile:
	gcc -c ./linux/system.c -o system.o -I./ -I./linux -DLINUX \
	    `pkg-config --cflags gtk+-2.0` 
	gcc -c ./linux/gtk2xtbin.c -o gtk2xtbin.o -I./ -I ./linux -DLINUX \
	    `pkg-config --cflags gtk+-2.0` 
	gcc -c flash.c -o flash.o -I./ -DLINUX
	gcc -c np_host.c -o np_host.o -I./ -DLINUX
	gcc -c flash_dll.c -o flash_dll.o -I./ -DLINUX
	gcc -c Test.c -o Test.o -I./ -DLINUX

#`pkg-config --cflags gtk+-2.0``pkg-config --libs gtk+-2.0`
