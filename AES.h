#include <stdint.h>

int turnSize(const int size);
void printState(int size, uint8_t state[4][turnSize(size)]);
void subBytes(const int size, uint8_t state[4][turnSize(size)], const uint8_t SBOX[16][16]);
void shiftRows(const int size, uint8_t state[4][turnSize(size)]);
