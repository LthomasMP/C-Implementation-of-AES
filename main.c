#include <stdio.h>
#include <stdlib.h>
#include "AES.c"

int main(){
    int size;
    printf("Block and key sizes ? 128, 192, or 256 bits\n");
    scanf("%d",&size);

    switch(size){
        case 128:
            printf("128 Not implemented yet :(\n");
            break;
        case 192:
            printf("192 Not implemented yet :(\n");
            break;
        case 256:
            printf("256 Not implemented yet :(\n");
            break;
    }

    return 0;
}