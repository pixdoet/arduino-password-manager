void generatePassword(char password_char[]);
void storePassword(int address, char title[], char password[]);

#pragma once
struct PasswordEntry
{
  char title[32];
  char password[16];
};


PasswordEntry getPasswordEntry();

void clearEeprom();