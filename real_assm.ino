#include "keypad_local.h"
#include "master_password.h"
#include "store_password.h"
#include "menu.h"

void setup()
{
  // keypad pins
  for (int i = 2; i <= 9; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }

  // display
  delay(500); // wait for init
  setup_screen();
  
  // Serial
  Serial.begin(9600);
}

void loop()
{
  bool unlocked = false;

  while (!unlocked)
  {
    bool menu_unlocked = unlock_menu();
    if (menu_unlocked)
    {
      unlocked = true;
    }
  }

  while (unlocked)
  {
    display_main_menu();
  }
}
