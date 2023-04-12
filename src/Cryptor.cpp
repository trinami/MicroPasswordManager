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
        bit1 = 0x1 & analogRead(A0);
        

        delayMicroseconds(200);
        bit2 = 0x1 & analogRead(A0);
        
        if(bit1 != bit2)
        {
            //Serial.print("+");
            return bit1;
        }
        //Serial.print(".");
        delayMicroseconds(300);
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
    for(uint8_t i = 0; i < 3; i++)
    {
      clearScreen();
      Serial.print("Detected first startup, generating new master key");
      for(uint8_t h = 0; h < i; h++)
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
  for(uint8_t i = 0; i < HASH_SIZE; i++)
  {
    iv[i] = B00000000;
    pass_hash[i] = B00000000;
    hashed[i] = B00000000;
    sha3.clear();
  }
}