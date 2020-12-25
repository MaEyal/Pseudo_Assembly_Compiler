myprog: f1.o f2.o f3.o list.o
	gcc -g -Wall -ansi -pedantic f1.o f2.o f3.o list.o -o myprog -lm
f1.o: f1.c h1.h
	gcc -c -Wall -ansi -pedantic f1.c -o f1.o
f2.o: f2.c
	gcc -c -Wall -ansi -pedantic f2.c -o f2.o
f3.o: f3.c
	gcc -c -Wall -ansi -pedantic f3.c -o f3.o -lm
list.o: list.c h1.h
	gcc -c -Wall -ansi -pedantic list.c -o list.o
