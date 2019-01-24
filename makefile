CC=gcc
CFLAGS=-Wall -Werror -Wextra
LDFLAGS=
EXEC=AES

all: $(EXEC)

AES: AES.o main.o
	$(CC) -o AES main.o $(LDFLAGS)

AES.o: AES.c
	$(CC) -o AES.o -c main.c $(CFLAGS)
	
main.o: main.c AES.h
	$(CC) -o main.o -c main.c $(CFLAGS)

clean:
	rm *.o

mrproper:
	rm $(EXEC)