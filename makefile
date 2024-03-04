HELL=/bin/bash
INCLUDE=-I../boole/src 
BIBLIO=-L../boole/src
OPTION = -O2
ifeq ($(shell hostname),serveur-imath01.univ-tln.fr)
	OPTION = -Wall -g
endif

ifeq ($(shell hostname),ou812.univ-tln.fr)
	OPTION = -Wall -g
endif
ifeq ($(shell hostname),port-gillot)
	OPTION = -Wall -g
endif

CFLAGS=$(OPTION) $(INCLUDE) $(BIBLIO)



all : apply.exe class.exe initial.exe 

#cover.exe gltoagl.exe check.exe initial.exe

special.exe :  option.o special.c
	gcc $(CFLAGS)  option.o special.c  -o $@  -lboole -lgmp

gltoagl.exe : gltoagl.c
	gcc $(CFLAGS) gltoagl.c  -o $@  -lboole -lgmp

option.o  : option.c
	gcc  $(CFLAGS) -c option.c
cover.exe :  option.o cover.c
	gcc $(CFLAGS)  option.o cover.c  -o $@  -lboole -lgmp

check.exe : option.o check.c
	gcc $(CFLAGS)  option.o check.c  -o $@  -lboole -lgmp

class.exe : option.o class.c
	gcc $(CFLAGS) option.o class.c  -o $@  -lboole -lgmp

apply.exe : option.o apply.c
	gcc $(CFLAGS) option.o apply.c  -o $@  -lboole -lgmp

initial.exe : option.o initial.c
	gcc $(CFLAGS) option.o initial.c  -o $@  -lboole -lgmp

lib :
	make -BC ../src
	make -B
clean :
	rm -f *.exe
