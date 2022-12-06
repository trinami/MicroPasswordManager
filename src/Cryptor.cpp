#include "Cryptor.h"

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

    for (byte charIndex = 2; charIndex < strlen(hexString); charIndex++)//skip 0x
    {
        bool oddCharIndex = charIndex & 1;

        if (oddLength)
        {
            // If the length is odd
            if (oddCharIndex)
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
            if (!oddCharIndex)
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
    for(int i = 0; i < arraySize; i++)
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
            for(int i = 2; i < size; i++)
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
    for(int i = 0; i < 7; i++)
    {
      clearScreen();
      Serial.print("Detected first startup, generating new master key");
      for(int h = 0; h < i/2; h++)
      {
        Serial.print('.');
      }
      Serial.println();
      for(int h = 0; h < HASH_SIZE; h++)
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
    Serial.print(pass_hash[0], HEX);
    Serial.print(pass_hash[1], HEX);
    Serial.print(pass_hash[2] >> 4, HEX);
    Serial.print("]");
}

void clearCryptor()
{
  //clear pass arrays
}