OPT=-O3 -Wall -std=c99
SRC=tic2html.c
INCLUDE=-I../../include/zlib
LIB=-lz

all: windows

windows: $(SRC)
	gcc $(OPT) $(SRC) $(INCLUDE) -L../../lib/mingw $(LIB) -o tic2html.exe

linux32: $(SRC)
	gcc $(OPT) $(SRC) $(INCLUDE) -L../../lib/linux32 $(LIB) -o tic2html

linux64: $(SRC)
	gcc $(OPT) $(SRC) $(INCLUDE) -L../../lib/linux $(LIB) -o tic2html

macosx: $(SRC)
	gcc $(OPT) $(SRC) $(INCLUDE) -L../../lib/macos $(LIB) -o tic2html
