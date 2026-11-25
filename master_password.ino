#include <EEPROM.h>

#include "keypad_local.h"

const char master_password[7] = "111111";

bool verifyMasterPassword()
{
  char pw_input[7];
  buildCharBuff(pw_input);

  Serial.print("PW Input: ");
  Serial.println(pw_input);
  if (strcmp(pw_input, master_password) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void updateMasterPassword(char new_master_password[])
{

}

void buildCharBuff(char password_buffer[])
{
  // char password_buffer[6];
  int i = 0;

  Serial.println("Input password: ");

  while (i < 6)
  {
    char keypress1 = get_key_press();
    if (keypress1)
    {
      if (keypress1 >= '0' && keypress1 <= '9')
      {
        password_buffer[i] = keypress1;
        Serial.println(keypress1);
        i++;
      }
      else
      {
        Serial.println("Non-number inputted!");
      }
    }
  }
  // add \0
  password_buffer[6] = '\0';
}