#include <EEPROM.h>

void generatePassword(char password_char[]) {
  /*
  Random password generator - randomly fill 16 chars from ASCII charset below
  */

  static const char passwordChars[75] = "1234567890!@#$%^&*()qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM./;";  // no ",", used for separating chars

  randomSeed(analogRead(0));  // "enhance randomness"

  for (int i = 0; i < 14; i++) {
    password_char[i] = passwordChars[random(0, 74)];
  }
  password_char[15] = '\0';
}

/*
  Allocate 32b for each password address
  16 * 4 * 5 = ~324byte

  store passwords in 1 - 3:
  0 - 64
  65 - 129
  130 - 194


  Alloc 32 for each title addr.
  Start from 256
  256 - 384
  385 - 513
  514 - 642
*/

const int address1 = 0;
const int address2 = 65;
const int address3 = 130;

// start at 256
const int title1 = 256;
const int title2 = 385;
const int title3 = 514;

int title_address;
int password_address;

void storePassword(int address, char title[], char password[]) {
  /*
  Store passwords into Arduino EEPROM. 5 entries only to fit <1kb
  - Format: "char title[64],char password[32]" (comma separated)
  */

  // check for valid address
  if (address < 1 || address > 3) {
    Serial.println("ERROR: Address is not between 1 and 3!");
    return;
  } 
  else 
  {
    // check if title length > 63 (safety)
    // I think this is how buffer overflows happen
    if (strlen(title) > 31) {
      Serial.println("ERROR: Title length must be under 31 characters!");
      Serial.println("Please restart the box");
      return;
    }
    else
    {
      // write into eeprom
      switch (address) {
        case 1:
          title_address = title1;
          password_address = address1;
          break;
        case 2:
          title_address = title2;
          password_address = address2;
          break;
        case 3:
          title_address = title3;
          password_address = address3;
          break;
      }

      PasswordEntry entry;
      strncpy(entry.title, title, sizeof(entry.title));
      strncpy(entry.password, password, sizeof(entry.password));
      EEPROM.put(title_address, entry.title);
      EEPROM.put(password_address, entry.password);
    }
  }
}

PasswordEntry getPasswordEntry(int address)
{
  switch (address) {
    case 1:
      title_address = title1;
      password_address = address1;
      break;
    case 2:
      title_address = title2;
      password_address = address2;
      break;
    case 3:
      title_address = title3;
      password_address = address3;
      break;
  }

  PasswordEntry entry;
  EEPROM.get(title_address, entry.title);
  EEPROM.get(password_address, entry.password);

  return entry;
}
void clearEeprom() {
  /*
  DANGEROUS: Clears everything stored in the EEPROM
  */
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}