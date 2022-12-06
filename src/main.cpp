#include <Arduino.h>

#include <avr/wdt.h>
#include <HID-Project.h>
#include "KeyboardDE.h"

void restart()
{
  for(int i = 0; i < 3; i++)
  {
    Serial.println(3-i);
    delay(1000);
  }
  delay(1);
  clearCryptor();
  wdt_enable(WDTO_15MS);
  
  while(true)
  {
    ;
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
  if(!isIntialized())
  {
    generateIv();
    ivToEEPROM();
    EEPROM.write(3*HASH_SIZE+1, key_count);//?
    intializeComplete();
    clearScreen();
    Serial.println("Generated intilization vector (IV).");
    Serial.println("Please note it on a secure location:\n");
    Serial.println(" ");
    Serial.println(" WARNING! This token is only visible once!\n");
    dumpByteArray(iv, HASH_SIZE);
    Serial.println(" ");
    anyKey();
  }
  else
  {
    switch(checkStorage)
    {
      case STORAGE_OK:
        Serial.print("Loaded Master Key");
        printMasterKey();
        Serial.println();
      break;
      case STORAGE_RESTORED:
        Serial.println("WARNING! Key was corrupt and tried to be restored");
        //continue or import
        //+intialize();
      break;
      case STORAGE_INVALID:
        Serial.println("ERROR: Key is corrupt and could not be restored");
        //+intialize();
        //import or delete
      break;
    }
  }
}

void loop()
{
  bool inMenu = true;
  while(inMenu)
  {
    //readAccounts
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
        //restart
      break;
      case '1':
        
      break;
      case '2':
        
      break;
      case '3':
        
      break;
      case '4':
        Serial.println("WARNING! All current keys will be gone!");
        Serial.println("If you have saved the key, you can restore it later with import");
        Serial.println("Write OVERRIDE to continue");
        if(confirmString("OVERRIDE", 8))
        {
          Serial.println("Please type in the key to import:");
          serialReadString(input, MAX_INPUT_SIZE, false);
          Serial.println();
          if(isValidHexTokenString(input, strlen(input)))
          {
            hexCharacterStringToBytes(iv, input);
            ivToEEPROM();
            Serial.println("Imported. Trying to restart...");
            restart();
          }
          else
          {
            Serial.println("Error, invalid Token!");
          }
        }
      break;
      case '5':
        Serial.println("WARNING! Deleting all keys, write DELETE to continue");
        if(confirmString("DELETE", 6)
        {
          for(int i = 0; i < HASH_SIZE*3+1; i++)
          {
            EEPROM.update(i, 0);
          }
          Serial.println("Cleared. Trying to restart...");
          restart();
        }
      break;
      default:
        Serial.println("Error type in a number from 0 to 5");
      break;
    }
  }
  
  
  clearScreen();
  Serial.println("Please type in password");
  serialReadString(input, MAX_INPUT_SIZE, true);
  sha3.reset();
  sha3.update(input, strlen(input));
  sha3.finalize(&pass_hash, HASH_SIZE);
  clearString(input, MAX_INPUT_SIZE);
  //dumpByteArray(pass_hash, HASH_SIZE);

  // xor pass iv
  for(int i = 0; i < HASH_SIZE; i++)
  {
    pass_hash[i] = pass_hash[i] ^ iv[i];
  }

  // loading bar, hash of hash
  for(int i = 0; i < 25; i++)
  {
    clearScreen();
    Serial.print('[');
    for(int h = 0; h < i+1; h++)
    {
      Serial.print('#');
    }
    for(int h = 0; h < 25-i-1; h++)
    {
      Serial.print(' ');
    }
    Serial.println(']');
    for(int h = 0; h < 12; h++) ///slow down 25*12
    {
      sha3.reset();
      sha3.update(&pass_hash, HASH_SIZE);
      sha3.finalize(&pass_hash, HASH_SIZE);
    }
  }
  clearScreen();

  printMasterKey();

  boolean running = true;
  while(running)
  {
    //clearScreen();
    Serial.println("Please type in name or /help for help");
    //while(Serial.available() == 0) {}

    clearString(input, MAX_INPUT_SIZE);
    serialReadString(input, MAX_INPUT_SIZE, false);

    if(strcmp(input, "/help") == 0)
    {
      Serial.println();
      Serial.println("Commands:");
      Serial.println(" /help - shows this command");
      Serial.println(" /clear - clears the screen");
      Serial.println(" /logout - logout and select other key");
      Serial.println();
    }
    else if(strcmp(input, "/clear") == 0)
    {
      clearScreen();
    }
    else if(strcmp(input, "/logout") == 0)
    {
      running = false;
      Serial.println();
      Serial.println("Logged out.");
      delay(2000);
    }
    else if(input[0] == '/')
    {
      char notFound[] = "Command not found: ";
      char all[19+MAX_INPUT_SIZE+1];
      strcpy(all, notFound);
      strcat(all, input);
      Serial.println();
      Serial.println(all);
    }
    else
    {
      sha3.reset();
      sha3.update(&input, strlen(input)); // hash input
      sha3.finalize(&hashed, HASH_SIZE);
      for(int i = 0; i < HASH_SIZE; i++)
      {
        hashed[i] = hashed[i] ^ pass_hash[i]; // xor pass^iv    hashedxtimes
      }
      
      sha3.reset();
      sha3.update(&hashed, HASH_SIZE); // hash xored 
      sha3.finalize(&hashed, HASH_SIZE);

///////
      char generated_pass[HASH_SIZE+1] = {0};
      for(int i = 0; i < HASH_SIZE; i++)
      {
        double num = (int)hashed[i];
        num *= 0.30196; //0.30196 * 255 = 76.999....  => int 76
        uint8_t idx = num;
        //Serial.print(charset[idx]);
        generated_pass[i] = charset[idx];
      }
      generated_pass[HASH_SIZE] = '\0';
      //Serial.println('\n');
      Serial.println();
      if(strlen(generated_pass) == HASH_SIZE)
      {
        Serial.println("[Copied] Press double CapsLock to paste!");
  
        BootKeyboard.begin();
        bool pasted = false;
        while(!pasted)
        {
          //warte bis led aus
          if(!(BootKeyboard.getLeds() & LED_CAPS_LOCK)) // led aus
          {
            while(!(BootKeyboard.getLeds() & LED_CAPS_LOCK))
            {
              delay(50);
              serialReadVoid();
            } // warte bis led an
            //Serial.println("on");
            long timestamp = millis();
            bool intime = true;
            while(BootKeyboard.getLeds() & LED_CAPS_LOCK) // solange an
            {
                if(millis() > timestamp + 250) // und nicht länger als
                {
                  intime = false;
                }
                serialReadVoid();
                //delay(1);
            }
            if(intime)
            {
              BootKeyboard.end();
              Keyboard2.begin();
              Keyboard2.print(generated_pass);
              delay(750);
              Keyboard2.end();
              Serial.println("[Pasted]");
              pasted = true;
              clearString(generated_pass, HASH_SIZE);
            }
            delay(50);
          }
        } //</pasting>
      }
      else
      {
        Serial.println("[Error Protection] Internal error, please use another name!");
      }
    }
  }
}