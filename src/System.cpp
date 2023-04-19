#include "System.h"
#include "Cryptor.h"

#include <avr/wdt.h>

void restart()
{
    Serial.end();
    
    clearCryptor();

    wdt_enable(WDTO_15MS);
    
    while(true)
    {
        ;
    }
}

void error(uint8_t code)
{
    clearCryptor();

    Serial.println("Error: ");
    switch(code)
    {
        case ERROR_STORAGE_INVALID:
            Serial.print("Storage invalid. Device has malfunctioned");
        break;
    }

    while(true)
    {
        delay(100000);
    }
}