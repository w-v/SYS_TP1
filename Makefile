#
# Link
#

CFLAGS = -Wall -g

bin/shell1: obj/shell2.o obj/shell1.o obj/shell3.o
	gcc $(CFLAGS) -o bin/shell1 obj/shell1.o obj/shell3.o obj/shell2.o

#
# Objets of TP Lists
#

obj/shell2.o: src/shell2.c
	gcc $(CFLAGS) -I./include -c src/shell2.c -o obj/shell2.o

obj/shell1.o: src/shell1.c
	gcc $(CFLAGS) -I./include -c src/shell1.c -o obj/shell1.o

obj/shell3.o: src/shell3.c
	gcc $(CFLAGS) -I./include -c src/shell3.c -o obj/shell3.o

#
# Remove files
#

clean :
	rm obj/*.o bin/*


