#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "AES.h"

int turnSize(const int size){
    switch (size){
        case 128:
            return 4;
            break;
        case 192:
            return 6;
            break;
        case 256:
            return 8;
            break;
        default:
            printf("Error size");
            break;
    }
}

void printState(int size, uint8_t state[4][turnSize(size)]){
    for (int i=0;i<4;i++){
        for (int j=0;j<(int)sizeof(state[0]);j++){
            printf("%x", state[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void subBytes(const int size, uint8_t state[4][turnSize(size)], const uint8_t SBOX[16][16]){
    for (int i=0;i<4;i++){
        for (int j=0;j<(int)sizeof(state[0]);j++){
            uint8_t b = (state[i][j]<<4);
            state[i][j] = SBOX[state[i][j]>>4][b>>4];

        }
    }
}

void shiftRows(const int size, uint8_t state[4][turnSize(size)]){
    uint8_t copy_state[4][turnSize(size)];
    for (int i=0;i<4;i++){
        for (int j=0;j<(int)sizeof(state[0]);j++){
            copy_state[i][j] = state[i][j];
        }
    }
    for (int i=1;i<4;i++){
        for (int j=0;j<(int)sizeof(state[0]);j++){
            state[i][j] = copy_state[i][(j+i)%turnSize(size)];
        }
    }

}