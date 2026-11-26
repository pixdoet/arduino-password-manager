#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include "store_password.h"
#include "keypad_local.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup_screen()
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

bool unlock_menu()
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("The Password Manager"));
  display.setCursor(0,10);
  display.println(F("Enter Master pwd: "));
  display.setCursor(0,20);
  display.println(F("Input is hidden"));
  display.display();

  bool unlock_device = verifyMasterPassword();
  if (!unlock_device)
  {
    Serial.println("Wrong password!");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F("Wrong password!"));
    display.setCursor(0,10);
    display.println(F("Enter Master pwd: "));
    display.setCursor(0,20);
    display.println(F("Input is hidden"));
    display.display();
  }

  return unlock_device; 
}

void display_main_menu()
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("A: Read passwords");
  display.setCursor(0,10);
  display.println("B: Save new password");
  display.setCursor(0,20);
  display.println("C: Clear all pwds");

  display.display();

  char keypress;
  bool mainMenuAsking = true;
  while (mainMenuAsking)
  {
    keypress = get_key_press();
    if (keypress)
    {
      switch (keypress)
      {
        case 'A':
          display_passwords();
          mainMenuAsking = false;
          break;
        case 'B':
          display_generate_passwords();
          mainMenuAsking = false;
          break;
        case 'C':
          display_clear_passwords();
          mainMenuAsking = false;
          break;
        default:
          Serial.print("Mainmenuasking: ");
          Serial.println(keypress);
      }
    }
  }
}

void display_passwords()
{
  bool displaying = true;
  while (displaying)
  {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F("View Passwords"));
    display.setCursor(0,10);
    display.println("Password 1,2,3?");
    display.setCursor(0,20);
    display.println(F("Press number key"));

    display.display();

    bool askingSelection = true;
    int address;
    char keypress;
    while (askingSelection)
    {
      keypress = get_key_press();
      if (keypress)
      {
        switch (keypress)
        {
          case '1':
            address = 1;
            askingSelection = false;
            break;
          case '5':
            address = 2;
            askingSelection = false;
            break;
          case '3':
            address = 3;
            askingSelection = false;
            break;
          default:
            Serial.println("Pls select a number");
            display.clearDisplay();
            display.setCursor(0,0);
            display.println(F("View Passwords"));
            display.setCursor(0,10);
            display.println(F("Press 1, 2 or 3"));
            display.display();
            break;
        }
      }
    }

    PasswordEntry entry = getPasswordEntry(address);

    display.clearDisplay();
    display.setCursor(0,0);
    display.print(F("Password number "));
    display.println(keypress);

    display.setCursor(0,10);
    display.println(entry.password);

    display.setCursor(0,20);
    display.println(F("Press D to exit"));
    display.display();

    bool waitingQuit = true;
    char keypress2;

    while (waitingQuit)
    {
      keypress2 = get_key_press();
      if (keypress2)
      {
        switch (keypress2)
        {
          case 'D':
            displaying = false;
            waitingQuit = false;
          default:
            Serial.print("Waitingquit: ");
            Serial.println(keypress2);
        }
      }
    }
  }
}

void display_generate_passwords()
{
  bool displaying = true;
  while (displaying)
  {
    Serial.println("Selecting which password to generate");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F("Gen new password"));
    display.setCursor(0,10);
    display.println(F("Set pw for 1,2,3?"));
    display.setCursor(0,20);
    display.println(F("Press number key"));
    display.display();

    bool askingSelection = true;
    int address;
    char keypress;
    while (askingSelection)
    {
      keypress = get_key_press();
      if (keypress)
      {
        switch (keypress)
        {
          case '1':
            address = 1;
            askingSelection = false;
            break;
          case '2':
          case '5':
          case '8':
            address = 2;
            askingSelection = false;
            break;
          case '3':
            address = 3;
            askingSelection = false;
            break;
          default:
            Serial.print("askingSelection_B: ");
            Serial.println(keypress);
        }
      }
    }

    char password[16];
    generatePassword(password);
    storePassword(address, "Password", password);

    PasswordEntry entry = getPasswordEntry(address);

    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F("Password saved!"));
    
    display.setCursor(0,10);
    display.print("Password is: ");
    display.println(entry.password);

    display.setCursor(0,24);
    display.println(F("Press D to exit"));
    display.display();

    bool waitingQuit = true;
    char keypress2;

    while (waitingQuit)
    {
      keypress2 = get_key_press();
      if (keypress2)
      {
        if (keypress2 == 'D')
        {
          displaying = false;
          waitingQuit = false;
        }
        else
        {
          Serial.print("Waitingquit: ");
          Serial.println(keypress2);
        }
      }
    }
  }
}

void display_clear_passwords()
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("Clearing pw..."));
  display.display();

  clearEeprom();

  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("All pw cleared!"));
  display.setCursor(0,20);
  display.println(F("Press D to exit"));
  display.display();

  bool waitingQuit = true;
  char keypress2;

  while (waitingQuit)
  {
    keypress2 = get_key_press();
    if (keypress2 == 'D')
    {
      waitingQuit = false;
    }
    else
    {

    }
  }
}