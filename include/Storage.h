#ifndef STORAGE_H
#define STORAGE_H

/*____  _                             
 / ___|| |_ ___  _ __ __ _  __ _  ___ 
 \___ \| __/ _ \| '__/ _` |/ _` |/ _ \
  ___) | || (_) | | | (_| | (_| |  __/
 |____/ \__\___/|_|  \__,_|\__, |\___|
                           |__*/

#include "Arduino.h"

#include "RStorage.h"


extern uint8_t iv[HASH_SIZE];

void writeIV();
void readIV();

#endif