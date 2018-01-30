#
# Link
#

CFLAGS = -Wall -g

bin/shell1: obj/shell1.o 
	gcc $(CFLAGS) -o bin/shell1 obj/shell1.o 

bin/shell2: obj/shell2.o 
	gcc $(CFLAGS) -o bin/shell2 obj/shell2.o

bin/shell3: obj/shell3.o 
	gcc $(CFLAGS) -o bin/shell3 obj/shell3.o

bin/shell4: obj/shell4.o 
	gcc $(CFLAGS) -o bin/shell4 obj/shell4.o

bin/shell5: obj/shell5.o 
	gcc $(CFLAGS) -o bin/shell5 obj/shell5.o

#
# Objets of TP Lists
#

obj/shell2.o: src/shell2.c
	gcc $(CFLAGS) -I./include -c src/shell2.c -o obj/shell2.o

obj/shell1.o: src/shell1.c
	gcc $(CFLAGS) -I./include -c src/shell1.c -o obj/shell1.o

obj/shell3.o: src/shell3.c
	gcc $(CFLAGS) -I./include -c src/shell3.c -o obj/shell3.o

obj/shell4.o: src/shell4.c
	gcc $(CFLAGS) -I./include -c src/shell4.c -o obj/shell4.o

obj/shell5.o: src/shell5.c
	gcc $(CFLAGS) -I./include -c src/shell5.c -o obj/shell5.o
#
# Remove files
#

clean :
	rm obj/*.o bin/*


