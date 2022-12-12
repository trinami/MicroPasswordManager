#ifndef RSTORAGE_H
#define RSTORAGE_H

#include "Arduino.h"
#include "System.h"

#define STORAGE_OK 0
#define STORAGE_RESTORED 1
#define STORAGE_NOT_INTIALIZED
#define STORAGE_INVALID 3

/*____          _                 _             _     ____  _                             
 |  _ \ ___  __| |_   _ _ __   __| | __ _ _ __ | |_  / ___|| |_ ___  _ __ __ _  __ _  ___ 
 | |_) / _ \/ _` | | | | '_ \ / _` |/ _` | '_ \| __| \___ \| __/ _ \| '__/ _` |/ _` |/ _ \
 |  _ <  __/ (_| | |_| | | | | (_| | (_| | | | | |_   ___) | || (_) | | | (_| | (_| |  __/
 |_| \_\___|\__,_|\__,_|_| |_|\__,_|\__,_|_| |_|\__| |____/ \__\___/|_|  \__,_|\__, |\___|
                                                                               |__*/

extern uint8_t keyCount;

void readKeyCount();
void setKeyCount(uint8_t count);
void resetKeyCount();
void eraseStorage();
void writeByte(uint8_t byte, uint16_t pos);
uint8_t readByte(uint16_t pos);
uint8_t checkByte(uint16_t pos);
uint8_t checkStorage();
void intializeStorage();
bool canGetIntialized();
bool isIntialized();

#endif