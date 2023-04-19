#ifndef CRYPTOR_H
#define CRYPTOR_H

/* ____                  _             
  / ___|_ __ _   _ _ __ | |_ ___  _ __ 
 | |   | '__| | | | '_ \| __/ _ \| '__|
 | |___| |  | |_| | |_) | || (_) | |   
  \____|_|   \__, | .__/ \__\___/|_|   
             |___/|*/

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
void hashToPass(uint8_t *hash, char *pass);

#endif