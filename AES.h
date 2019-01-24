#include <stdint.h>
#ifndef AES_H_
#define AES_H_

int turnSize(int size);
void printState(int size, uint8_t state[4][turnSize(size)]);
void stateFromFile(const int size, uint8_t state[4][turnSize(size)], char* file_name);

void subBytes(int size, uint8_t state[4][turnSize(size)], const uint8_t SBOX[16][16]);
void shiftRows(int size, uint8_t state[4][turnSize(size)]);
void mixColumns(int size, uint8_t state[4][turnSize(size)]);
void addRounkKey(int size, uint8_t state[4][turnSize(size)], uint8_t keys[turnSize(size)*11][4], int round);
void keySchedule(int size, uint8_t keys[turnSize(size)*11][4], const uint8_t key[4][turnSize(size)], const uint8_t SBOX[16][16], const uint8_t RCON[10][4]);

void createCipherFile(const int size, uint8_t state[4][turnSize(size)],const char* cipher_file);

#endif