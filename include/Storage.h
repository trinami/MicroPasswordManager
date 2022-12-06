#ifndef STORAGE_H
#define STORAGE_H

#define STORAGE_SIZE 512
#define STORAGE_OK 0
#define STORAGE_RESTORED 1
#define STORAGE_INVALID 2

#define MAX_KEY_COUNT 3


#include "Arduino.h"
#include <EEPROM.h>

uint8_t key_count = 0;

int checkStorage();

#endif