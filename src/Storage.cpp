#include "Storage.h"

#include "Cryptor.h"
#include "RStorage.h"

uint8_t iv[HASH_SIZE] = {0};


void readStorage()
{
    readIV();
}

void writeIV()
{
    for(uint8_t i = 0; i < HASH_SIZE; i++)
    {
        writeByte(1+i, iv[i]);
    }
}

void readIV()
{
    for(uint8_t i = 0; i < HASH_SIZE; i++)
    {
        iv[i] = readByte(1+i);
    }
}