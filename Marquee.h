//============================================================================================================
//
//  LED Project for Under Glow -- (c) 2021 John Nguyen. All Rights Reserved.
//
//  File:           Marquee.H                                                                                
//                                                                                                         
//  Descriptoin:    Draws an old movie theater style marquee with two colors
//                                                                          
//  History:        December 21, 2021                              
//                             
//=============================================================================================================

#include <Arduino.h>
#define  FASTLED_INTERNAL
#include <FastLED.h>

void DrawMarquee()
{
    CRGB c;
    for (int i = 0; i < NUM_PIXEL; i++)
        g_LEDs[i] = CRGB::DarkGreen;

    static int scroll = 0;
    scroll++;

    for (int i = scroll % 5; i < NUM_PIXEL - 1; i += 5)
        g_LEDs[i] = CRGB::Aqua;
    delay(55);

    for (int i = scroll % 10; i < NUM_PIXEL - 1; i += 10)
        g_LEDs[i] = CRGB::White;
    delay(55);
}
