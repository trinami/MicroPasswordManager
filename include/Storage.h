#ifndef STORAGE_H
#define STORAGE_H

#include "Arduino.h"

#include "RStorage.h"

extern uint8_t iv[HASH_SIZE];

void readStorage();
void writeIV();
void readIV();
void addKey(const char *key);
void removeKey(uint8_t keyPosition);
void importMasterKey(const char* mkey);
bool checkIfKeyExist(const char* key);
void moveKey(uint8_t oldPosition, uint8_t newPosition);

#endif