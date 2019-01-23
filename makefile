all: AES

AES: AES.o main.o
	gcc -o AES AES.o main.o

AES.o: AES.c
	gcc -o AES.o -c main.c -W -Wall -Wextra -Werror
	
main.o: main.c AES.h
	gcc -o main.o -c main.c -W -Wall -Wextra -Werror

clean:
	rm *.o

mrproper:
	rm AES