CC = gcc
CFLAGS = -Wall -std=gnu11
OUTPUT = automat

$(OUTPUT): automat.o calc.o
	$(CC) $(CFLAGS) automat.o calc.o -o $(OUTPUT)

calc.o: calc.c
	$(CC) $(CFLAGS) -c calc.c -o calc.o

automat.o: automat.c
	$(CC) $(CFLAGS) -c automat.c -o automat.o

clean:
	rm *.o $(OUTPUT)