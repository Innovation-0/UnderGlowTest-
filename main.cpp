//============================================================================================================
//
//  LED Project for Under Glow -- (c) 2021 John Nguyen. All Rights Reserved.
//
//  File:           main.cpp                                                                                
//                                                                                                         
//  Descriptoin:    Main Source file to run program in the ESP32 Dev Board. 
//                                                                          
//  History:        December 20, 2021                              
//                             
//=============================================================================================================

#include <Arduino.h>
#define  FASTLED_INTERNAL
#include <FastLED.h>
#include "BluetoothSerial.h" 

//Bluetooth Parameters
BluetoothSerial Serial_BT; 
enum btnInput {NONE, ForestGreen, Teal, SpringGreen};
btnInput currentInput = NONE;

// Constants ESP32 Board
#define RED_PIN            16
#define GREEN_PIN          17
#define BLUE_PIN           18  

// Constanst for LEDS
#define NUM_PIXEL          50
#define LED_PIN            5
CRGB g_LEDs[NUM_PIXEL] = {0};                                                  // Frame buffer for FastLED




// LED Preset Effects
#include "NightRider.h"
#include "Marquee.h"
#include "Bounce.h"



// Functions
void BoardLED()
{
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
}


// Boot up Settings
void setup() 
{
  // Bluetooth Setup
  Serial.begin(115200);
  Serial_BT.begin("ESP32 LED Controller");                                         //Name of your Bluetooth interface -> will show up on your phone

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  //BoardLED();

  FastLED.addLeds<WS2811, LED_PIN, BRG>(g_LEDs, NUM_PIXEL).setCorrection(TypicalLEDStrip);                     // Adds our LED strip to the FastLED library
  FastLED.setBrightness(128);                                                    // Brightness for LED Strip
}

void loop() 
{
 BouncingBallEffect balls(NUM_PIXEL, 8, 28, true);
 

  if (Serial_BT.available()) 
    {
    char buttonPress = (char)Serial_BT.read();
    
      switch(buttonPress)
      {
        case '1': 
          currentInput = ForestGreen;
          break;
        case '2':  
          currentInput = Teal;
          break;
        case '3': 
          currentInput = SpringGreen;
          break;
    }
    Serial.println(currentInput);
    }

  if (currentInput == 1)
    {
      while(true)
      {
        balls.Draw();
      FastLED.show();
      }
    }
      
  else if (currentInput == 2)
    {
      FastLED.clear();
      DrawMarquee();
      FastLED.show();
    }

  else if (currentInput == 3)
      {
        FastLED.clear();
        DrawNightRider();
        FastLED.show(); 
      }

}
  