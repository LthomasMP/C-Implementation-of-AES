void subBytes(int **state, int size, int **SBOX);
void shiftRows(int **state, int size);
void mixColumns(int **state, int size);
void addRoundKey(int **state, int size, int ***key);

void keySchedule(int ***key, int size, int **SBOX, int **RCON);