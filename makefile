hexeditplus: hexeditplus.c 
	gcc -m32 -g -Wall hexeditplus.c -o hexeditplus

.PHONY: clean

clean:
	rm -f *.o hexeditplus
