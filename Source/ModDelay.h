/*
  ==============================================================================

    ModDelay.h
    Created: 6 Jun 2023 6:40:36pm
    Author:  Gus Anthon

  ==============================================================================
*/

#pragma once

#define SmoothingCoefficient_Fine 0.002


#include "LFO.h"
#include "RCLowPass.h"
#include "JuceHeader.h"

class ModDelay
{
public:

    ModDelay();
    ~ModDelay();

    void prepare(double inSampleRate);

    void reset();

    void process(float* inAudio, float inTime, float inFeedback, int waveShaperType, float inWetDry, float gain, float* inModBuffer,float* outAudio, int inNumSamplesToRender);
    
    void setWaveShaper(int choice);

float processSample(float inSample, float inTime, float inFeedback, int waveShaperType, float inWetDry, float gain, float inModulation);
    
    RCLowpass fdbackLPF;

private:

    double getInterpolatedSample(float inDelayTimeInSamples);
    
    double mSampleRate;
    double mBuffer[maxDelayBufferSize];
    int mBufferSize;

    double mFeedbackSample;

    float mTimeSmoothed;

    int mDelayIndex;
   
    int mWaveShaperType;
    
    static inline float BBD_approx(float inputSample) {
        return (inputSample - (pow(inputSample, 2) / 8.0f) - (pow(inputSample, 3) / 16.0f));// + 0.125f);
    }

    
    
    //chebyshev polynomials of the first kind
    static float T_2(float x)
    {
        return 2 * x * x - 1.0f;
    }
    static float T_3(float x)
    {
        return (4.0f * pow(x, 3)) - 3.0f * x;
    }
    static float T_4(float x)
    {
        return (8.0f * pow(x, 4)) - (8.0f * x * x) + 1.0f;
    }
    
    static float tube_clip(float x)
    {
        return x + juce::Decibels::decibelsToGain(-42.0f)*T_2(x)
            + juce::Decibels::decibelsToGain(-68.0f) * T_3(x) + juce::Decibels::decibelsToGain(-84.0f) * T_4(x);
    }
    
    static int signum(float value) {
        if (value > 0)
            return 1;
        else if (value < 0)
            return -1;
        else
            return 0;
    }


    static float V1(float Vin) {
        
        return (std::abs(Vin) > 0.6000) ? (0.8333f * Vin - 0.5000f * signum(Vin)) : 0.0f;
    }

    static float V2(float Vin) {
    
        return (std::abs(Vin) > 2.9940) ? (0.3768f * Vin - 1.1281f * signum(Vin)) : 0.0f;
    }

    static float V3(float Vin) {
        
        return (std::abs(Vin) > 5.4600) ? (0.2829f * Vin - 1.5446f * signum(Vin)) : 0.0f;
    }

    static float V4(float Vin) {
        
        return (std::abs(Vin) > 1.8000) ? (0.5743f * Vin - 1.0338f * signum(Vin)) : 0.0f;
    }

    static float V5(float Vin) {
        return (std::abs(Vin) > 4.0800) ? (0.2673f * Vin - 1.0907f * signum(Vin)) : 0.0f;
    }

    static float BuchlaFolder(float Vin) {
//        float V1n = V1(Vin);
//        float V2n = V2(Vin);
//        float V3n = V3(Vin);
//        float V4n = V4(Vin);
//        float V5n = V5(Vin);

        return -12.f * V1(Vin) - 27.777f * V2(Vin) - 21.428f * V3(Vin) + 17.647f * V4(Vin) + 36.363f * V5(Vin) + 5.f * Vin;
    }
    
};
