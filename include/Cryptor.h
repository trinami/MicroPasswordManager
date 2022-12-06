#ifndef CRYPTOR_H
#define CRYPTOR_H

#define HASH_SIZE 32

#include "Arduino.h"
#include <Crypto.h>
#include <SHA3.h>

SHA3_256 sha3;
uint8_t iv[HASH_SIZE];
uint8_t pass_hash[HASH_SIZE];
uint8_t hashed[HASH_SIZE];

const char* charset = "!#$%&+-/123456789:<=>?@ABCDEFGHIJKLMNPQRSTUVWXYZ[\\]^_abcdefghijkmnpqrstuvwxyz";

#endif