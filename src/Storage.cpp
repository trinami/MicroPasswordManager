#include "Storage.h"

#include "Cryptor.h"
#include "RStorage.h"

uint8_t iv[HASH_SIZE] = {0};


void readStorage()
{
    readKeyCount();
    readIV();
}

void writeIV()
{
    for(uint8_t i = 0; i < HASH_SIZE; i++)
    {
        writeByte(iv[i], 2+i);
    }
}

void readIV()
{
    for(uint8_t i = 0; i < HASH_SIZE; i++)
    {
        iv[i] = readByte(2+i);
    }
}

void addKey(const char *key)
{

}

void removeKey(uint8_t keyPosition);
void importMasterKey(const char* mkey);
bool checkIfKeyExist(const char* key);
void moveKey(uint8_t oldPosition, uint8_t newPosition);

///////////// add key?