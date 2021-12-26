//============================================================================================================
//
//  LED Project for Under Glow -- (c) 2021 John Nguyen. All Rights Reserved.
//
//  File:           NightRder.h                                                                               
//                                                                                                         
//  Descriptoin:    Draws a Comet with Fading Effect, Like the Night Rider but with color
//                                                                          
//  History:        December 21, 2021                              
//                             
//=============================================================================================================

#include <Arduino.h>
#define  FASTLED_INTERNAL
#include <FastLED.h>


void DrawNightRider()
{
    const byte fadeAmt = 64;                                // Fraction of 256 to fad a pixel by, if it is chosen to be faded in the pass
    const int cometSize = 4;                                // Size of the comet in pixels
    const double cometSpeed = 0.5;                          // How far to advance teh comet every frame

    static int iDirection = 1;                              // Current direction (-1 or +1)
    static double iPos = 0.0;                               // Current comet position on strip

    iPos += iDirection * cometSpeed;                        // Update the comet position

    // Flip the comet direction when ity hits either end
    if (iPos == (NUM_PIXEL - cometSize) || iPos == 0)
        iDirection *= -1;

    // Draw the comet at its current position
    for (int i = 0; i < cometSize; i++)
        g_LEDs[(int)iPos + i] = CRGB::DarkGreen;

    // Fade the LEDs one step
    for (int j = 0; j <NUM_PIXEL; j++)
        if (random(4) == 2)
        g_LEDs[j] = g_LEDs[j].fadeLightBy(fadeAmt);

    delay(30);

}