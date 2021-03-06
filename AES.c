#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

int turnSize(int size){
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
    return 0;
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

void stateFromFile(int size, uint8_t state[4][turnSize(size)], char* file_name){
    FILE* f;
    int sizeFile = 10;
    char* str = (char*)calloc(sizeFile,sizeof(int));
    f = fopen(file_name, "r");
    if(f == NULL){
        perror("Error opening file\n");
    }
    if (fgets(str,size,f) != NULL){
        int l=0;
        for (int i=0;i<4;i++){
            for (int j=0;j<turnSize(size);j++){
                if(str[i] != '\0'){
                    state[i][j] = str[l];
                    l++;
                    sizeFile++;
                }
            } 
        }
    }
    fclose(f);
}


void subBytes(int size, uint8_t state[4][turnSize(size)], const uint8_t SBOX[16][16]){
    for (int i=0;i<4;i++){
        for (int j=0;j<(int)sizeof(state[0]);j++){
            uint8_t b = (state[i][j]<<4);
            state[i][j] = SBOX[state[i][j]>>4][b>>4];

        }
    }
}

void shiftRows(int size, uint8_t state[4][turnSize(size)]){
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

void mixColumns(int size, uint8_t state[4][turnSize(size)]){
    int i;
    /*
     * Multiplication in Gallois field
     */ 
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

void addRounkKey(int size, uint8_t state[4][turnSize(size)], uint8_t keys[turnSize(size)*11][4], int round){
    for (int i=0;i<4;i++){
        for (int j=0;j<turnSize(size);j++){
            state[i][j] = state[i][j] ^ keys[turnSize(size)*round+j][i];
        }
    }
}

void keySchedule(int size, uint8_t keys[turnSize(size)*11][4], const uint8_t key[4][turnSize(size)], const uint8_t SBOX[16][16], const uint8_t RCON[10][4]){
    /*
     * Round Key 0 = Cipher key
     * Consider keys as colums of 4 elments
     */
    for (int i=0;i<4;i++){
        for (int j=0;j<(int)sizeof(keys[0]);j++){
            keys[i][j]=key[j][i];
        }
    }

    /*
     * Other roucdkeys
     */
    for (int l=4;l<11*turnSize(size);l++){
        /*
         * First column of round keys
         */
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

        /*
         * Other Columns
         */
        else{
            for (int i=0;i<4;i++){
                keys[l][i]=keys[l-4][i] ^ keys[l-1][i];
            }
        }
    }
}

void createCipherFile(int size, uint8_t state[4][turnSize(size)], const char* cipher_file){
    /*
     *  Create cipher.txt
     */

    /* 
     * Put in cipher.txt content of cipherState
     */
    FILE* f = fopen(cipher_file,"w");
    if (f != NULL){
        for (int i=0;i<4;i++){
            for (int j=0;j<turnSize(size);j++){
                fprintf(f,"%x",state[i][j]);
            }
        }
    }
    fclose(f);
}

