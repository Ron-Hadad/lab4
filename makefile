hexeditplus: hexeditplus.c
	gcc -m32 -Wall hexeditplus.c -o hexeditplus

.PHONY: clean

clean:
	rm -f *.o hexeditplus











# all:
# 	gcc -Wall -g -m32 ./units.c -o units

# .PHONY: clean
	
# clean:
# 	rm -f ./units
