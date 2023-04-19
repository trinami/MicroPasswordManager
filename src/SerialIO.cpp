#include "SerialIO.h"

#define HID_CUSTOM_LAYOUT
#define LAYOUT_GERMAN
#include "HID-Project.h"

#include "System.h"
#include "Cryptor.h"
#include "Storage.h"
#include "RStorage.h"

char input[MAX_INPUT_SIZE+1] = {0};

void intialize()
{
    generateIv();
    writeIV();
    clearScreen();
    Serial.println("Generated intilization vector (IV).");
    Serial.println("Please note it on a secure location:\n");
    Serial.println(" ");
    Serial.println(" WARNING! This token is only visible once!\n");
    dumpByteArray(iv, HASH_SIZE);
    Serial.println(" ");
    anyKey();
    writeByte(0, INTIALIZED);
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
    for(uint8_t i = 0; i < MAX_INPUT_SIZE; i++)
    {
        Serial.print(' ');
    }
    Serial.print('\r');
}

void serialReadVoid()
{
    while(Serial.available() > 0)
    {
        Serial.read();
    }
}

void serialReadString(char *string, int size, bool isPassword)
{
    clearString(string, size);
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
            for(uint8_t i = 0; i < strlen(string); i++)
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
    for(uint8_t i = 0; i < size; i++)
    {
        string[i] = B01010101;
        delay(1);
        string[i] = B10101010;
    }
    string[0] = '\0';
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
    
    serialReadVoid();
}

uint8_t getNumber()
{
    const char *numbers = "0123456789";
    uint8_t number = 0;
    bool invalidNumber = true;

    while(invalidNumber)
    {
        serialReadString(input, 1, false);
        for(uint8_t i = 0; i < 10; i++)
        {
            if(numbers[i] == input[0])
            {
                number = i;
                invalidNumber = false;
            }
        }
    }
    
    return number;
}

void importMKey()
{
    Serial.println("Please type in the key to import:");
    serialReadString(input, HASH_SIZE*2 + 2, false);
    Serial.println();
    if(isValidHexTokenString(input, strlen(input)))
    {
        hexCharacterStringToBytes(iv, input);
        writeIV();
        Serial.println("Imported. Trying to restart...");
        restartCountdown();
    }
    else
    {
        Serial.println("Error, invalid Token!");
    }
    anyKey();
}

void restartCountdown()
{
    for(uint8_t i = 0; i < 3; i++)
    {
        Serial.println(3-i);
        delay(1000);
    }
    restart();
}

void masterKeyInput()
{
	serialReadString(input, MAX_INPUT_SIZE, true);
	sha3.reset();
	sha3.update(input, strlen(input));
	sha3.finalize(&pass_hash, HASH_SIZE);
	clearString(input, MAX_INPUT_SIZE);
	 

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
	    for(int h = 0; h < 25; h++) ///slow down 25*12 /////////////////// return char in serial read only if valid from charset SECURITY ISSUE??
	    {
	        sha3.reset();
	        sha3.update(&pass_hash, HASH_SIZE);
	        sha3.finalize(&pass_hash, HASH_SIZE);
	    }
	}
	clearScreen();
}

void login()
{
    clearScreen();
    Serial.println("Please type in Key");
    masterKeyInput();
        
    //menu();
    printMasterKey();
    menu();
}

void menu()
{
    boolean inMenu = true;
    while(inMenu)
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
            Serial.println(" /logout - logout and return to main menu");
            Serial.println();
        }
        else if(strcmp(input, "/clear") == 0)
        {
            clearScreen();
        }
        else if(strcmp(input, "/logout") == 0)
        {
            inMenu = false;
            clearCryptor();
            Serial.println();
            Serial.println("Logged out.");
            delay(2000);
        }
        else if(input[0] == '/')
        {
            char notFound[] = "Command not found: ";/////////attack possible?
            char all[19+MAX_INPUT_SIZE+1];
            strcpy(all, notFound);
            strcat(all, input);
            Serial.println();
            Serial.println(all);
        }
        else // clean up code
        {
            sha3.reset();
            sha3.update(&input, strlen(input)); // hash input
            sha3.finalize(&hashed, HASH_SIZE);
            for(int i = 0; i < HASH_SIZE; i++)
            {
                hashed[i] = hashed[i] ^ pass_hash[i]; // xor pass^iv        hashedxtimes
            }
            
            sha3.reset();
            sha3.update(&hashed, HASH_SIZE); // hash xored 
            sha3.finalize(&hashed, HASH_SIZE);

///////
            char generated_pass[HASH_SIZE+1] = {0};
            hashToPass(hashed, generated_pass);
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
                        unsigned long timestamp = millis();
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
                            Keyboard.begin();
                            delay(50);
                            Keyboard.print(generated_pass);
                            delay(400);
                            Keyboard.end();
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