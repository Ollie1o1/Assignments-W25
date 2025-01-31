RaczkaOliverA1: RaczkaOliverA1.o RaczkaOliverA1Main.o
	gcc RaczkaOliverA1.o RaczkaOliverA1Main.c -o RaczkaOliverA1
RaczkaOliverA1.o: RaczkaOlvierA1.c givenA1.h
	gcc -Wall -std=c99 -c RaczkaOliverA1.c
RaczkaOliverA1Main.o: RaczkaOliver1Main.c givenA1.h
	gcc -Wall -std=c99 -c RaczkaOliverA1Main.c
clean:
	rm *.o RaczkaOliverA1