assembler: symbolLinkedList.o assemblerFunct.o assemblerData.o main.o firstScan.o secondScan.o
	gcc -g -ansi -Wall -pedantic symbolLinkedList.o assemblerFunct.o assemblerData.o firstScan.o secondScan.o main.o  -o assembler
main.o: main.c symbolLinkedList.h assemblerData.h assemblerFunct.h firstScan.h secondScan.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
assemblerFunct.o: assemblerFunct.c symbolLinkedList.h assemblerData.h 
	gcc -c -ansi -Wall -pedantic assemblerFunct.c -o assemblerFunct.o
symbolLinkedList.o: symbolLinkedList.c  assemblerData.h
	gcc -c -ansi -Wall -pedantic symbolLinkedList.c -o symbolLinkedList.o
assemblerData.o: assemblerData.c
	gcc -c -ansi -Wall -pedantic assemblerData.c -o assemblerData.o
firstScan.o: firstScan.c symbolLinkedList.h assemblerFunct.h assemblerData.h
	gcc -c -ansi -Wall -pedantic firstScan.c -o firstScan.o 
secondScan.o: secondScan.c symbolLinkedList.h assemblerFunct.h assemblerData.h
	gcc -c -ansi -Wall -pedantic secondScan.c -o secondScan.o 
