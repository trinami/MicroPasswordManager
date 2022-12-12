#include <Arduino.h>

#include "Storage.h"
#include "Cryptor.h"
#include "SerialIO.h"
#include "RStorage.h"

void invalidMenu()
{
    bool inMenu = true;
    while(inMenu)
    {
        clearScreen();
        Serial.println("ERROR: Key is corrupt and could not be restored");
        Serial.println("");
        Serial.println("");
        Serial.println("Please choose:");
        Serial.println("");
        Serial.println(" (1) Restore (Import) Master Key");
        Serial.println(" (2) Reset Device");
        Serial.println("");
        Serial.println(" (0) Exit");
        Serial.println("");
        uint8_t choose = getNumber();
        switch(choose)
        {
            case 0:
                restart();
            break;
            case 1:
                importMKey();
            break;
            case 2:
                eraseStorage();
                Serial.println("Erased Storage. Trying to restart...");
                restartCountdown();
            break;
        }
    }
}

void restoredMenu()
{
    bool inMenu = true;
    while(inMenu)
    {
        clearScreen();
        Serial.println("WARNING! Key was corrupt and tried to be restored");
        Serial.println("");
        Serial.println("");
        Serial.println("Please choose:");
        Serial.println("");
        Serial.println(" (1) Continue");
        Serial.println(" (2) Restore (Import) Master Key");
        Serial.println("");
        Serial.println(" (0) Exit");
        Serial.println("");
        uint8_t choose = getNumber();
        switch(choose)
        {
            case 0:
                restart();
            break;
            case 1:
                inMenu = false;
            break;
            case 2:
                importMKey();
            break;
        }
    }
}

void setup()
{
    //terminal connected, master key intialized, storage okay, 
    Serial.begin(9600);
    while(!Serial)
    {
        ;
    }
    delay(5000);

    if(isIntialized())
    {
        switch(checkStorage())
        {
            case STORAGE_INVALID:
                invalidMenu();
            break;
            case STORAGE_RESTORED:
                restoredMenu();
            break;
        }
    }
    else
    {
        intialize();
    }

    readStorage();//isnt that intialize?? or just readiv because keyvount read?
}

void loop()
{
    //readAccounts in setup, => keycount is known
    clearScreen();
    Serial.println("Please choose:");
    Serial.println();
    Serial.println(" (1) Login");
    Serial.println(" (2) Add Account");
    Serial.println(" (3) Delete Account");
    Serial.println(" (4) Import Master Key ");
    Serial.println(" (5) Delete Master Key and Keys");
    Serial.println();
    Serial.println(" (0) Exit");
    char choose[1+1] = {0};
    serialReadString(choose, 1, false);
    Serial.println();
    switch(choose[0])
    {
        case '0':
            //EEPROM.write(3*HASH_SIZE+1, key_count)
            //overwrite buffers
            clearCryptor();
            restart();
        break;
        case '1':
            login();
        break;
        case '2':
            addAccount(); // input in serialio, generate in cryptor and add key in storage?!
        break;
        case '3':
            delAccount();
        break;
        case '4':
            Serial.println("WARNING! All current keys will be gone!");
            Serial.println("If you have saved the key, you can restore it later with import");
            Serial.println("Write OVERRIDE to continue");
            if(confirmString("OVERRIDE", 8))
            {
                importMKey();
            }
        break;
        case '5':
            Serial.println("WARNING! Deleting all keys, write DELETE to continue");
            if(confirmString("DELETE", 6))
            {
                eraseStorage();
                Serial.println("Cleared. Trying to restart...");
                restart();
            }
        break;
        default:
            Serial.println("Error type in a number from 0 to 5");
        break;
    }
}