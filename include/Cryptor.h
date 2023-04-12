#ifndef CRYPTOR_H
#define CRYPTOR_H

#include "Arduino.h"
#include "Crypto.h"
#include "SHA3.h"

#include "System.h"
#include "Storage.h"

extern const char* charset;

extern SHA3_256 sha3;
extern uint8_t pass_hash[HASH_SIZE];
extern uint8_t hashed[HASH_SIZE];

boolean getRandomBit();
uint8_t getRandomByte();
void hexCharacterStringToBytes(byte *byteArray, const char *hexString);
void dumpByteArray(const byte *byteArray, const byte arraySize);
byte nibble(char c);
bool isValidHexTokenString(const char *hexString, int size);
void generateIv();
void printMasterKey();
void clearCryptor();

#endif