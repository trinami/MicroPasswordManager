#ifndef SYSTEM_H
#define SYSTEM_H

/*____            _                 
 / ___| _   _ ___| |_ ___ _ __ ___  
 \___ \| | | / __| __/ _ \ '_ ` _ \ 
  ___) | |_| \__ \ ||  __/ | | | | |
 |____/ \__, |___/\__\___|_| |_| |_|
        |__*/

#include "Arduino.h"

// Storage Settings
#define STORAGE_SIZE 1024

// Security Settings
#define HASH_BIT_SIZE 256
#define KEY_SIZE 3

// IO Settings
#define MAX_INPUT_SIZE 64+2
#define PASSWORD_CHARSET "!#$%&+-/123456789:<=>?@ABCDEFGHIJKLMNPQRSTUVWXYZ[\\]^_abcdefghijkmnpqrstuvwxyz"
#define INTIALIZED B10101010
#define NOT_INTIALIZED B01010101

// Error Codes
#define ERROR 0
#define ERROR_STORAGE_INVALID 1

// Don't edit
#define RSTORAGE_SIZE STORAGE_SIZE/3
const int HASH_SIZE = HASH_BIT_SIZE/8;
#define MAX_KEY_COUNT (RSTORAGE_SIZE - 2 - HASH_SIZE)/KEY_SIZE


void restart();
void error(uint8_t code);

#endif