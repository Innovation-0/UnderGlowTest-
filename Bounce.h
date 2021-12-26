//============================================================================================================
//
//  LED Project for Under Glow -- (c) 2021 John Nguyen. All Rights Reserved.
//
//  File:           Bounce.H                                                                                
//                                                                                                         
//  Descriptoin:    Bouncing Ball Effect on LED Strip
//                                                                          
//  History:        December 21, 2021                              
//                             
//=============================================================================================================

#include <sys/time.h>                                                       // For Time-of-Day
#include <Arduino.h>
#define  FASTLED_INTERNAL
#include <FastLED.h>

using namespace std;
#include <vector>

extern CRGB g_LEDs [];

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))                               // Count elements in a static array

static const CRGB ballColors [] =
{
    CRGB::DarkGreen,
    CRGB::ForestGreen,
    CRGB::SpringGreen,
    CRGB::Teal
};

class BouncingBallEffect
{
    private:

    double InitialBallSpeed(double height) const
    {
        return sqrt(-2 * Gravity * height);                                 // Because MATH!  
    }
    size_t cLength;
    size_t cBalls;
    byte   fadeRate;
    bool   bMirrored;

    const double Gravity = -9.81;                                           // Becasue PHYSICS!
    const double StartHeight = 1;                                           // Drop balls from max  height initially
    const double ImpactVelocity = InitialBallSpeed(StartHeight);
    const double SpeedKnob = 4.0;                                           // Higher valuses will slow the effect

    vector<double> ClockTimeAtLastBounce, Height, BallSpeed, Dampening;
    vector<CRGB>   Colors;

    static double Time()
    {
        timeval tv = { 0 };
        gettimeofday(&tv, nullptr);
        return (double)(tv.tv_usec / 1000000.0 + (double) tv.tv_sec);
    }

    public:

    // BouncingBallEffect
    //
    // Caller spec strip length, number of balls, persistence level (255 is least), and whether the ball SHould be drawn mirrored from each side.

    BouncingBallEffect(size_t cLength, size_t ballCount = 8, byte fade = 28, bool bMirrored = false)
        :cLength(cLength - 1),
         cBalls(ballCount),
         fadeRate(fade),
         bMirrored(bMirrored),
         ClockTimeAtLastBounce(ballCount),
         Height(ballCount),
         BallSpeed(ballCount),
         Dampening(ballCount),
         Colors(ballCount)
   {
    for (size_t i = 0; i < ballCount; i++)
        {
            Height[i]                = StartHeight;                             // Curent Ball Height
            ClockTimeAtLastBounce[i] = Time();                                  // When ball last hit ground state
            Dampening[i]             = 0.90 - i / pow(cBalls, 2);               // Bounciness of this ball
            BallSpeed[i]             = InitialBallSpeed(Height[i]);             // Don't dampen inital launch
            Colors[i]                = ballColors[i % ARRAYSIZE(ballColors)];
        }
   } 
    
    // Draw each of the balls. When any ball settles with too little energy, it is "kicked" to restart it

    virtual void Draw()
    {
        
        if (fadeRate != 0)
        {
            for (size_t i = 0; i < cLength; i++)
                g_LEDs[i].fadeLightBy(fadeRate);
        }
        else
            FastLED.clear();
        
        // Draw each of the balls
        for (size_t i = 0; i < cBalls; i++)
        {
            double TimeSinceLastBounce = (Time() - ClockTimeAtLastBounce[i]) / SpeedKnob;

            Height[i] = 0.5 * Gravity * pow(TimeSinceLastBounce, 2.0) + BallSpeed[i] * TimeSinceLastBounce;

            // Ball Hits ground and Bounce
            if (Height[i] < 0)
            {
                Height[i] = 0;
                BallSpeed[i] = Dampening[i] * BallSpeed[i];
                ClockTimeAtLastBounce[i] = Time();

                if (BallSpeed[i] < 0.01)
                    BallSpeed[i] = InitialBallSpeed(StartHeight) * Dampening[i];
            }

            size_t position = (size_t)(Height[i] * (cLength - 1) / StartHeight);

            g_LEDs[position]     += Colors[i];
            g_LEDs[position + 1] += Colors[i];

            if (bMirrored)
            {
                g_LEDs[cLength - 1 - position] += Colors[i];
                g_LEDs[cLength - position]     += Colors[i];
            }

        }
         delay(30);
    }
    
};

