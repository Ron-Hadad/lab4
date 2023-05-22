all: hexeditplus task4

hexeditplus: task1.c 
	gcc -m32 -g -Wall task1.c -o hexeditplus

task4: task4.c
	gcc -m32 -fno-pie -fno-stack-protector task4.c -o task4

.PHONY: clean

clean:
	rm -f *.o hexeditplus task4
