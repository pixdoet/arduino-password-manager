#include <EEPROM.h>

#include "keypad_local.h"

const char master_password[7] = "111111"; // CHANGE HERE

bool verifyMasterPassword()
{
  char pw_input[7];
  buildCharBuff(pw_input);

  Serial.print("Master password input: ");
  Serial.println(pw_input);

  if (strcmp(pw_input, master_password) == 0)
  {
    return true; // password correct
  }
  else
  {
    return false; // password wrong
  }
}

void buildCharBuff(char password_buffer[])
{
  int i = 0;

  Serial.println("Please input password: ");

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
  // add null terminator for password input
  password_buffer[6] = '\0';
}