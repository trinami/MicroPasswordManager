#include "Storage.h"

bool isIntialized()
{
    if(EEPROM.read(0) == B10101010 && EEPROM.read(1) == B10101010 && EEPROM.read(2) == B10101010)
    {
        return true;
    }

    return false;
}

bool intializeComplete()
{
    EEPROM.write(0, B10101010);
    EEPROM.write(1, B10101010);
    EEPROM.write(2, B10101010);
    //key_count = EEPROM.read(3*HASH_SIZE+1);
}

int checkStorage()
{
    int returnCode = STORAGE_OK;

    for(uint16_t i = 0; i < (STORAGE_SIZE / 3)-1; i++)
    {
        uint8_t byte1 = EEPROM.read(i*3+0);
        uint8_t byte2 = EEPROM.read(i*3+1);
        uint8_t byte3 = EEPROM.read(i*3+2);
        
        if(byte1 != byte2 || byte2 != byte3 || byte1 != byte3)
        {
            returnCode = STORAGE_RESTORED;

            if(byte1 == byte2)
            {
                EEPROM.write(i*3+2, byte1);
            }
            else if(byte2 == byte3)
            {
                EEPROM.write(i*3+0, byte2);
            }
            else if(byte1 == byte3)
            {
                EEPROM.write(i*3+1, byte3);
            }
            else
            {
                return STORAGE_INVALID;
            }
        }
    }

    return returnCode;
}

void ivToEEPROM()
{
    for(int i = 0; i < HASH_SIZE; i++)
    {
        EEPROM.update(i+1, iv[i]);
    }
    for(int i = 0; i < HASH_SIZE; i++)
    {
        EEPROM.update(i+1+HASH_SIZE, iv[i]);
    }
    for(int i = 0; i < HASH_SIZE; i++)
    {
        EEPROM.update(i+1+2*HASH_SIZE, iv[i]);
    }
}