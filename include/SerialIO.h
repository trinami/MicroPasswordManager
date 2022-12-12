#ifndef SERIALIO_H
#define SERIALIO_H

#include "Arduino.h"

#include "System.h"

extern char input[MAX_INPUT_SIZE+1];

void intialize();
void printHex(uint8_t num);
void clearScreen();
void clearLine();
void serialReadVoid();
void serialReadString(char *string, int size, bool isPassword);
void clearString(char *string, int size);
bool confirmString(const char *string, int size);
void anyKey();
uint8_t getNumber();
void importMKey();
void restartCountdown();
void login();
void addAccount();
void delAccount();
void menu();

#endif