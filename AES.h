#include <stdint.h>

int turnSize(const int size);
void printState(int size, uint8_t state[4][turnSize(size)]);
void stateFromFile(const int size, uint8_t state[4][turnSize(size)], char* file_name);

void subBytes(const int size, uint8_t state[4][turnSize(size)], const uint8_t SBOX[16][16]);
void shiftRows(const int size, uint8_t state[4][turnSize(size)]);
void mixColumns(const int size, uint8_t state[4][turnSize(size)]);
void addRounkKey(const int size, uint8_t state[4][turnSize(size)], uint8_t keys[turnSize(size)*11][4], int round);
void keySchedule(const int size, uint8_t keys[turnSize(size)*11][4], const uint8_t key[4][turnSize(size)], const uint8_t SBOX[16][16], const uint8_t RCON[10][4]);

void createCipherFile(const int size, uint8_t state[4][turnSize(size)],const char* cipher_file);
