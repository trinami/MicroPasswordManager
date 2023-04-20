# Arduino Micro Password Manager

created with Platformio & C++
The stick only stores the IV, no password! The IV is stored 3 times with auto error correction. The generated password is a result of multiple SHA-3 256 rounds of the IV, the password and the term.

## Instructions
0. flash
1. Connect via Serial e.g. with minicom
2. Note the IV
3. Type in a password
4. Type in a term
5. Press two times CapsLock to paste a password!
