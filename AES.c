#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "AES.h"

#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

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

void mixColumns(const int size, uint8_t state[4][turnSize(size)]){
    int i;
    unsigned char Tmp,Tm,t;
    for(i = 0; i < turnSize(size); i++) {
        t = state[0][i];
        Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i] ;
        Tm = state[0][i] ^ state[1][i] ; Tm = xtime(Tm); state[0][i] ^= Tm ^ Tmp ;
        Tm = state[1][i] ^ state[2][i] ; Tm = xtime(Tm); state[1][i] ^= Tm ^ Tmp ;
        Tm = state[2][i] ^ state[3][i] ; Tm = xtime(Tm); state[2][i] ^= Tm ^ Tmp ;
        Tm = state[3][i] ^ t ; Tm = xtime(Tm); state[3][i] ^= Tm ^ Tmp ;
    }
}

void addRounkKey(const int size, uint8_t state[4][turnSize(size)], uint8_t keys[turnSize(size)*11][4], int round){
    for (int i=0;i<4;i++){
        for (int j=0;j<turnSize(size);j++){
            state[i][j] = state[i][j] ^ keys[turnSize(size)*round+j][i];
        }
    }
}

void keySchedule(const int size, uint8_t keys[turnSize(size)*11][4], const uint8_t key[4][turnSize(size)], const uint8_t SBOX[16][16], const uint8_t RCON[10][4]){
    // Round Key 0 = Cipher key
    // Consider keys as colums of 4 elments
    for (int i=0;i<4;i++){
        for (int j=0;j<sizeof(keys[0]);j++){
            keys[i][j]=key[j][i];
        }
    }

    //Other roucdkeys
    for (int l=4;l<11*turnSize(size);l++){
        //First column of round keys
        if (l%4 == 0){
            uint8_t rotWord[4];
            for (int i=0;i<4;i++){
                rotWord[i]=keys[l-1][(i+1)%turnSize(size)];
            }
            uint8_t subBytesRotWord[4];
            for (int i=0;i<4;i++){
                uint8_t b = (rotWord[i]<<4);
                subBytesRotWord[i]=SBOX[rotWord[i]>>4][b>>4];
            }
            for (int i=0;i<4;i++){
                keys[l][i]=keys[l-4][i] ^ subBytesRotWord[i] ^ RCON[(l-4)/4][i];
            }
        }

        // Other Columns
        else{
            for (int i=0;i<4;i++){
                keys[l][i]=keys[l-4][i] ^ keys[l-1][i];
            }
        }
    }
}
