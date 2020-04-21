CC = gcc
CFLAGS = -Wall -g
OBJECTS = bitio.o main.o

bitio: $(OBJECTS) 
	$(CC) $(CFLAGS) -o bitio $(OBJECTS)

bitio.o: bitio.c bitio.h
	$(CC) $(CFLAGS) -c bitio.c -o bitio.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f bitio $(OBJECTS) *~

format:
	find *.c *.h | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l80 -nut
