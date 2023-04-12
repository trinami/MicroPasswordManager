#ifndef STORAGE_H
#define STORAGE_H

#include "Arduino.h"

#include "RStorage.h"

extern uint8_t iv[HASH_SIZE];

void readStorage();
void writeIV();
void readIV();
void importMasterKey(const char* mkey);

#endif