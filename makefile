all: AES

AES: main.o
	gcc -o AES main.o

main.o: main.c
	gcc -c main.c

clean:
	rm main.o 