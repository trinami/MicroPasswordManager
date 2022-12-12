#include "System.h"

#include <avr/wdt.h>

void restart()
{
    wdt_enable(WDTO_15MS);
    
    while(true)
    {
        ;
    }
}

void error(uint8_t code)
{
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