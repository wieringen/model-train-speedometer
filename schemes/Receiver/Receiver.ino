/*
  Receiver.pde

  Model Train Speed-o-meter, http://www.baijs.com

  Copyright (c) 2014, Maarten Baijs
  All rights reserved.

*/

#include <math.h>
#include <JeeLib.h>
#include <PortsLCD.h>

LiquidCrystal lcd(6, 5, 3, 7, 8, 9);
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;

const char *LCD_LINE_SPLASH_1 = "Speed-o-meter   ";
const char *LCD_LINE_SPLASH_2 = "baijs.com       ";
const char *LCD_LINE_NO_DATA = "No data recieved.";
const char *LCD_LINE_CONNECTING = "Connecting...   ";

const int RF12B_NETWORK = 33;
const int RF12B_NODE_ID = 1;

Port led_receiving_data (1);

MilliTimer timer_receiving_data_freq;

typedef struct
{
    float kmh;
    float mph;
    float wheel_revisions;

} Train_data;

Train_data train_data;

const int UNIT_SWITCH_PIN = A5;

void setup ()
{
    // Initialize serial debugging.
    //
    Serial.begin(9600);

    // Initialize the button to switch between metric and imperial.
    //
    pinMode(UNIT_SWITCH_PIN, INPUT);

    // Initialize rf12 tranceiver.
    //
    rf12_initialize(RF12B_NODE_ID, RF12_868MHZ, RF12B_NETWORK);

    // Setup the LCD's number of rows and columns.
    //
    lcd.begin(LCD_COLUMNS, LCD_ROWS);
    lcd.clear();

    // Display splash screen.
    //
    lcd.setCursor(0,0);
    lcd.println(LCD_LINE_SPLASH_1);

    lcd.setCursor(0,1);
    lcd.println(LCD_LINE_SPLASH_2);

    // After 3 seconds clear splash message and show that we are ready to receive data.
    //
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println(LCD_LINE_CONNECTING);
}

void loop ()
{
    if (rf12_recvDone() && rf12_crc == 0)
    {
        //Serial.print("Recieving");

        // Since we are receiving data we can reset the data timer to 0.
        //
        timer_receiving_data_freq.set(0);

        // Copies the rf12data to the inData var.
        //
        memcpy(&train_data, (byte*) rf12_data, sizeof train_data);

        print_speed();

        toggle_receive_led(1);
        delay(100);
        toggle_receive_led(0);
    }

    // If we dont receive data for 5 seconds display message.
    //
    if(timer_receiving_data_freq.poll(5000))
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(LCD_LINE_NO_DATA);
    }
}

static void print_speed ()
{
  lcd.clear();
  lcd.setCursor(0,0);

  // Display speed in KM.
  //
  if (digitalRead(UNIT_SWITCH_PIN) == HIGH)
  {
    lcd.print(round(train_data.kmh));
    lcd.setCursor(9,0);
    lcd.print(" KM/H");
  }
  // Display speed in MPH.
  //
  else
  {
    lcd.print(round(train_data.mph));
    lcd.setCursor(9,0);
    lcd.print(" MPH");
  }

  // Display revisions of the wheel on the second line.
  //
  lcd.setCursor(0,1);
  lcd.print(round(train_data.wheel_revisions));
  lcd.setCursor(9,1);
  lcd.print(" MS/rev");
}

static void toggle_receive_led (byte on)
{
    led_receiving_data.mode2(OUTPUT);
    led_receiving_data.digiWrite2(on);
}
