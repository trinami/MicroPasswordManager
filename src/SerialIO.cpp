#include "SerialIO.h"

void printHex(uint8_t num)
{
    char hexCar[2];

    sprintf(hexCar, "%02X", num);
    Serial.print(hexCar);
}

void clearScreen()
{
    Serial.write(27);             // ESC command
    Serial.print("[2J");        // clear screen command
    Serial.write(27);
    Serial.print("[H"); 
}

void clearLine()
{
    Serial.print('\r');
    for(int i = 0; i < MAX_INPUT_SIZE; i++)
    {
        Serial.print(' ');
    }
    Serial.print('\r');
}

void serialReadVoid()
{
    while(Serial.available() > 0)
    {
        char x = Serial.read();
    }
}

void serialReadString(char *string, int size, bool isPassword)
{
    serialReadVoid();
    int cursor = 0;
    char character = 0;
    do
    {
        while(Serial.available() <= 0)
        {
            ;//wait until character received
        }
        while(Serial.available() > 0)//für jeden charakter solange 
        {
            character = Serial.read();
            if((character == 8 || character == 127 ) && cursor > 0)
            {
                cursor--;
                string[cursor] = '\0';
            }
            else
            {
                if(isPrintable(character) && cursor < size)
                {
                    string[cursor] = character;
                    cursor++;
                    string[cursor] = '\0';///////new
                }
            }
            
        }
        clearLine();
        if(isPassword)
        {
            for(int i = 0; i < strlen(string); i++)
            {
                Serial.print('*');
            }
        }
        else
        {
            Serial.print(string); 
        }
    }
    while(character != 13);
}

void clearString(char *string, int size)
{
    for(int i = 0; i < size; i++)
    {
        string[i] = B10101010;
        delay(1);
    }
    //string[0] = '\0';
    memset(string, 0, sizeof(string));
    string = {0};
}


bool confirmString(const char *string, int size)
{
    char checkString[size+1] = {0};
    serialReadString(checkString, size, false);
    Serial.println();
    if(strcmp(checkString, string) == 0)
    {
        return true;
    }
    Serial.println("Abording.");
    return false;
}

void anyKey()
{
    Serial.println(" ");
    Serial.println("Press any key to continue");

    while(Serial.available() <= 0)
    {
      ;
    }
    
    char x = Serial.read();
}