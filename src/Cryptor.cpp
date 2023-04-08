#include "Cryptor.h"

#include "SerialIO.h"


SHA3_256 sha3;
uint8_t pass_hash[HASH_SIZE];
uint8_t hashed[HASH_SIZE];

boolean getRandomBit()
{
    while(true)
    {
        boolean bit1, bit2;
        if(analogRead(A0) % 2 == 1)
        {
            bit1 = true;
        }
        else
        {
            bit1 = false;
        }
        
        delay(2);
        
        if(analogRead(A0) % 2 == 1)
        {
            bit2 = true;
        }
        else
        {
            bit2 = false;
        }
        
        if(bit1 != bit2)
        {
            return bit1;
        }
        delay(3);
    }
}

uint8_t getRandomByte()
{
    uint8_t rnd = 0;
    for(uint16_t i = 1; i < 256; i*=2)
    {
        if(getRandomBit())
        {
            rnd += i;
        }
    }
    return rnd;
}


// by johnwasser https://forum.arduino.cc/t/hex-string-to-byte-array/563827/3
void hexCharacterStringToBytes(byte *byteArray, const char *hexString)
{
    bool oddLength = strlen(hexString) & 1;

    byte currentByte = 0;
    byte byteIndex = 0;

    for(byte charIndex = 2; charIndex < strlen(hexString); charIndex++)//skip 0x
    {
        bool oddCharIndex = charIndex & 1;

        if(oddLength)
        {
            // If the length is odd
            if(oddCharIndex)
            {
                // odd characters go in high nibble
                currentByte = nibble(hexString[charIndex]) << 4;
            }
            else
            {
                // Even characters go into low nibble
                currentByte |= nibble(hexString[charIndex]);
                byteArray[byteIndex++] = currentByte;
                currentByte = 0;
            }
        }
        else
        {
            // If the length is even
            if(!oddCharIndex)
            {
                // Odd characters go into the high nibble
                currentByte = nibble(hexString[charIndex]) << 4;
            }
            else
            {
                // Odd characters go into low nibble
                currentByte |= nibble(hexString[charIndex]);
                byteArray[byteIndex++] = currentByte;
                currentByte = 0;
            }
        }
    }
}

void dumpByteArray(const byte *byteArray, const byte arraySize)
{
    Serial.print("0x");
    for(uint8_t i = 0; i < arraySize; i++)
    {
        if(byteArray[i] < 0x10)
        {
            Serial.print("0");
        }
        
        Serial.print(byteArray[i], HEX);
    }
}

byte nibble(char c)
{
    if(c >= '0' && c <= '9')
    {
        return c - '0';
    }

    if(c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }

    if(c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }

    return 0;
}


bool isValidHexTokenString(const char *hexString, int size)
{
    if(size == 2 + (HASH_SIZE*2))
    {
        if(hexString[0] == '0' && hexString[1] == 'x')
        {
            for(uint8_t i = 2; i < size; i++)
            {
                if(!((hexString[i] >= '0' && hexString[i] <= '9') || (hexString[i] >= 'a' && hexString[i] <= 'f') || (hexString[i] >= 'A' && hexString[i] <= 'F')))//not a hex char
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void generateIv()
{
    sha3.reset();
    for(uint8_t i = 0; i < 7; i++)
    {
      clearScreen();
      Serial.print("Detected first startup, generating new master key");
      for(uint8_t h = 0; h < i/2; h++)
      {
        Serial.print('.');
      }
      Serial.println();
      for(uint8_t h = 0; h < HASH_SIZE; h++)
      {
        iv[i] = getRandomByte();
      }
      sha3.update(&iv, HASH_SIZE);
    }
    sha3.finalize(&iv, HASH_SIZE);
}

void printMasterKey()
{
    Serial.print("[");
    //Serial.print(pass_hash[0], HEX);
    //Serial.print(pass_hash[1], HEX);
    //Serial.print(pass_hash[2] >> 4, HEX);
    dumpByteArray(pass_hash, 3);
    Serial.print("]");
    Serial.println("");
}

void clearCryptor()
{
  //clear pass arrays
  
}

boolean checkKey(uint8_t byte1, uint8_t byte2, uint8_t byte3) //check if given key already exist
{
    for(uint8_t i = 0; i < keyCount; i++)
    {
        if(readByte(32+1+1+i*KEY_SIZE+0) == byte1 &&readByte(32+1+1+i*KEY_SIZE+1) == byte2 && readByte(32+1+1+i*KEY_SIZE+2) == byte3)
        {
            return true;
        }
    }

    return false;
}

void addKey(uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
    writeByte(32+1+1+keyCount*KEY_SIZE+0, byte1);
    writeByte(32+1+1+keyCount*KEY_SIZE+1, byte2);
    writeByte(32+1+1+keyCount*KEY_SIZE+2, byte3);
    keyCount++;
    setKeyCount(keyCount);
}

void moveKey(uint8_t pos_old, uint8_t pos_new)
{
    uint8_t tmp_key[KEY_SIZE] = {0};

    for(uint8_t i = 0; i < 3; i++)
    {
        tmp_key[i] = readByte(32+1+1+pos_old*KEY_SIZE+i);
        writeByte(32+1+1+pos_old*KEY_SIZE+i, readByte(32+1+1+pos_new*KEY_SIZE+i));
        writeByte(32+1+1+pos_new*KEY_SIZE+i, tmp_key[i]);
    }
}

void delKey(uint8_t pos)
{
    
    /*fange bei pos an
    höre bei keyCount-2 auf
    {
        bewege i+1 auf i
    }*/

    for(uint8_t i = pos; i < keyCount-2; i++)
    {
        moveKey(i+1, i);
    }

    for(uint8_t i = 0; i < KEY_SIZE; i++)
    {
        writeByte(pos, B11111111);
        writeByte(pos, B00000000);
    }
    
    keyCount--;
    setKeyCount(keyCount);

    //überschreibe keycount-1 mit 0
    //ändere keycount um -1

}
/*
kc 2 pos 1
( ) ( ) (x)
(x) ( ) ( )
(a) ( ) ( )
(b) (x) ( )
 1   3   0
 */