#include "RStorage.h"

#include "EEPROM.h"

void eraseStorage()
{
    for(uint16_t i = 0; i < STORAGE_SIZE; i++)
    {
        EEPROM.update(i, B10101010);
        EEPROM.write(i, B01010101);
    }
}

void writeByte(uint16_t pos, uint8_t byte)
{
    EEPROM.update(pos*3+0, byte);
    EEPROM.update(pos*3+1, byte);
    EEPROM.update(pos*3+2, byte);
}

uint8_t readByte(uint16_t pos)
{
    uint8_t byte1 = EEPROM.read(pos*3+0);
    uint8_t byte2 = EEPROM.read(pos*3+1);
    uint8_t byte3 = EEPROM.read(pos*3+2);
    
    if(byte1 == byte2 && byte2 == byte3 && byte3 == byte1)
    {
        return byte1;
    }
    else
    {
        error(STORAGE_INVALID);
        return false;
    }
}

uint8_t checkByte(uint16_t pos)
{
    uint8_t byte1 = EEPROM.read(pos*3+0);
    uint8_t byte2 = EEPROM.read(pos*3+1);
    uint8_t byte3 = EEPROM.read(pos*3+2);

    uint8_t returnCode = STORAGE_OK;

    if(byte1 != byte2 || byte2 != byte3 || byte1 != byte3)
    {
        returnCode = STORAGE_RESTORED;

        if(byte1 == byte2)
        {
            EEPROM.write(pos*3+2, byte1);
        }
        else if(byte2 == byte3)
        {
            EEPROM.write(pos*3+0, byte2);
        }
        else if(byte1 == byte3)
        {
            EEPROM.write(pos*3+1, byte3);
        }
        else
        {
            return STORAGE_INVALID;
        }
    }

    return returnCode;
}

//checks storage and repairs
uint8_t checkStorage()
{
    uint8_t returnCode = STORAGE_OK;

    for(uint16_t i = 0; i < 1+HASH_SIZE; i++)
    {
        uint8_t byteReturnCode = checkByte(i);
        if(byteReturnCode > returnCode)
        {
            returnCode = byteReturnCode;
            if(byteReturnCode == STORAGE_INVALID)
            {
                return STORAGE_INVALID;
            }
        }
    }

    return returnCode;
}

void intializeStorage()
{
    eraseStorage();
    writeByte(0, INTIALIZED);
}

bool isIntialized()
{
    if(checkByte(0) != STORAGE_INVALID)
    {
        if(readByte(0) == INTIALIZED)
        {
            return true;
        }
    }

    return false;
}